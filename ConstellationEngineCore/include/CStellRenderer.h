#pragma once

#include "Minimal.h"
#include "CStellWindow.h"
#include "CStellDevice.h"
#include "CStellSwapChain.h"

#include <cassert>
#include <memory>
#include <vector>

namespace CStell
{
	class CStellRenderer
	{
	public:
		CStellRenderer(CStellWindow &window, CStellDevice &device);
		~CStellRenderer();

		CStellRenderer(const CStellRenderer&) = delete;
		CStellRenderer& operator=(const CStellRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return m_CStellSwapChain->getRenderPass(); }
		float getAspectRatio() const { return m_CStellSwapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return m_isFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const
		{
			assert(m_isFrameStarted && "Cannot get command buffer when frame not in progress");
			return m_commandBuffers[m_currentFrameIndex];
		}

		int GetFrameIndex() const
		{
			assert(m_isFrameStarted && "Cannot get frame index when frame not in progress");
			return m_currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffer();
		void freeCommandBuffers();
		void recreateSwapChain();

		CStellWindow& m_CStellWindow;
		CStellDevice& m_CStellDevice;
		std::unique_ptr<CStellSwapChain> m_CStellSwapChain;
		std::vector<VkCommandBuffer> m_commandBuffers;

		uint32_t m_currentImageIndex;
		int m_currentFrameIndex;
		bool m_isFrameStarted;
	};
}