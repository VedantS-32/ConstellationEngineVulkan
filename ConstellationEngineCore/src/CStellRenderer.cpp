#include "CStellRenderer.h"

#include <array>
#include <cassert>
#include <stdexcept>

namespace CStell
{
	CStellRenderer::CStellRenderer(CStellWindow& window, CStellDevice& device) :
		m_CStellWindow(window),
		m_CStellDevice(device),
		m_IsFrameStarted(false),
		m_CurrentFrameIndex(0)
	{
		RecreateSwapChain();
		CreateCommandBuffer();
	}
	CStellRenderer::~CStellRenderer() { FreeCommandBuffers(); }	

	VkCommandBuffer CStellRenderer::BeginFrame()
	{
		assert(!m_IsFrameStarted && "Can't call beginFrame while already in progress");

		auto result = m_CStellSwapChain->acquireNextImage(&m_CurrentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		m_IsFrameStarted = true;

		auto commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void CStellRenderer::EndFrame()
	{
		assert(m_IsFrameStarted && "Can't call endFrame while is not in progress");

		auto commandBuffer = GetCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}

		auto result = m_CStellSwapChain->submitCommandBuffers(&commandBuffer, &m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			m_CStellWindow.WasWindowResized())
		{
			m_CStellWindow.ResetWindowResizedFlag();
			RecreateSwapChain();
		}

		else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

		m_IsFrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % CStellSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void CStellRenderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(m_IsFrameStarted && "Can't call beginSwapChain if frame is not in progress");
		assert(commandBuffer == GetCurrentCommandBuffer() && 
			"Can't begin render pass on command buffer from a different frame");


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_CStellSwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_CStellSwapChain->getFrameBuffer(m_CurrentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_CStellSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.05f, 0.05f, 0.05f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_CStellSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_CStellSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_CStellSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void CStellRenderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(m_IsFrameStarted && "Can't call endSwapChain if frame is not in progress");
		assert(commandBuffer == GetCurrentCommandBuffer() &&
			"Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void CStellRenderer::CreateCommandBuffer()
	{
		m_CommandBuffers.resize(CStellSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CStellDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_CStellDevice.device(), &allocInfo, m_CommandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void CStellRenderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(
			m_CStellDevice.device(),
			m_CStellDevice.getCommandPool(),
			static_cast<uint32_t>(m_CommandBuffers.size()),
			m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	void CStellRenderer::RecreateSwapChain()
	{
		auto extent = m_CStellWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = m_CStellWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_CStellDevice.device());

		if (m_CStellSwapChain == nullptr)
		{
			m_CStellSwapChain = std::make_unique<CStellSwapChain>(m_CStellDevice, extent);
		}
		else
		{
			std::shared_ptr<CStellSwapChain> oldSwapChain = std::move(m_CStellSwapChain);
			m_CStellSwapChain = std::make_unique<CStellSwapChain>(m_CStellDevice, extent, oldSwapChain);

			if (!oldSwapChain->CompareSwapFormat(*m_CStellSwapChain.get()))
			{
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
		}
	}
}
