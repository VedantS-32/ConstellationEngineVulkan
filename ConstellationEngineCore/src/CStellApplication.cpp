#include "CStellApplication.h"

#include <array>
#include <stdexcept>

namespace CStell
{
	CStellApplication::CStellApplication() :
		m_CStellWindow(m_Width, m_Height, "Constellation Engine"),
		m_CStellDevice(m_CStellWindow),
		m_CStellSwapChain(m_CStellDevice, m_CStellWindow.getExtent())
	{
		CreatePipelineLayout();
		CreatePipeline();
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
		auto pipelineConfig =
			CStellShaderPipeline::m_DefaultPipelineConfigInfo(m_CStellSwapChain.width(), m_CStellSwapChain.height());
		pipelineConfig.m_RenderPass = m_CStellSwapChain.getRenderPass();
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
		m_CommandBuffers.resize(m_CStellSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_CStellDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_CStellDevice.device(), &allocInfo, m_CommandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (int i = 0; i < m_CommandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_CStellSwapChain.getRenderPass();
			renderPassInfo.framebuffer = m_CStellSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_CStellSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			m_CStellShaderPipeline->bind(m_CommandBuffers[i]);
			vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_CommandBuffers[i]);
			if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("Failed to record command buffer!");
			}
		}
	}

	void CStellApplication::DrawFrame()
	{
		uint32_t imageIndex;
		auto result = m_CStellSwapChain.acquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		result = m_CStellSwapChain.submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}
	}
}
