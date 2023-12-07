#include "CStellApplication.h"

#include <array>
#include <stdexcept>

namespace CStell
{
	CStellApplication::CStellApplication() :
		m_CStellWindow(m_Width, m_Height, "Constellation Engine"),
		m_CStellDevice(m_CStellWindow)
	{
		LoadModels();
		CreatePipelineLayout();
		RecreateSwapChain();
		CreateCommandBuffer();
	}
	CStellApplication::~CStellApplication()
	{
		vkDestroyPipelineLayout(m_CStellDevice.device(), m_PipelineLayout, nullptr);
	}

	void CStellApplication::Run()
	{
		while (!m_CStellWindow.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(m_CStellDevice.device());
	}

	void CStellApplication::LoadModels()
	{
		//std::vector<CStellModel::m_Vertex> vertices{ {{0.0f, -0.5}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}} };
		std::vector<CStellModel::m_Vertex> vertices{
			//Triangle coords  //RGB values
			{{  0.0f, -0.5f }, {1.0f, 0.0f, 0.0f}},
			{{  0.5f,  0.5f }, {0.0f, 1.0f, 0.0f}},
			{{ -0.5f,  0.5f }, {0.0f, 0.0f, 1.0f}} };
		m_CStellModel = std::make_unique<CStellModel>(m_CStellDevice, vertices);
	}

	void CStellApplication::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(m_CStellDevice.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void CStellApplication::CreatePipeline()
	{
		assert(m_CStellSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		CStellShaderPipeline::m_DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.m_RenderPass = m_CStellSwapChain->getRenderPass();
		pipelineConfig.m_PipelineLayout = m_PipelineLayout;
		m_CStellShaderPipeline = std::make_unique<CStellShaderPipeline>(
			m_CStellDevice,
			pipelineConfig,
			"shader/vert.spv",
			"shader/frag.spv"
		);
	}

	void CStellApplication::CreateCommandBuffer()
	{
		m_CommandBuffers.resize(m_CStellSwapChain->imageCount());

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

	void CStellApplication::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(
			m_CStellDevice.device(),
			m_CStellDevice.getCommandPool(),
			static_cast<float>(m_CommandBuffers.size()),
			m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	void CStellApplication::DrawFrame()
	{
		uint32_t imageIndex;
		auto result = m_CStellSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		RecordCommandBuffer(imageIndex);
		result = m_CStellSwapChain->submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			m_CStellWindow.WasWindowResized())
		{
			m_CStellWindow.ResetWindowResizedFlag();
			RecreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}
	}
	
	void CStellApplication::RecreateSwapChain()
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
			m_CStellSwapChain = std::make_unique<CStellSwapChain>(m_CStellDevice, extent, std::move(m_CStellSwapChain));
			if (m_CStellSwapChain->imageCount() != m_CommandBuffers.size())
			{
				FreeCommandBuffers();
				CreateCommandBuffer();
			}
		}
		CreatePipeline();
	}

	void CStellApplication::RecordCommandBuffer(int imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_CStellSwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_CStellSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_CStellSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_CStellSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_CStellSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_CStellSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

		m_CStellShaderPipeline->bind(m_CommandBuffers[imageIndex]);
		m_CStellModel->bind(m_CommandBuffers[imageIndex]);
		m_CStellModel->draw(m_CommandBuffers[imageIndex]);

		vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
		if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to record command buffer!");
		}
	}
}
