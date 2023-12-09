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

		VkRenderPass GetSwapChainRenderPass() const { return m_CStellSwapChain->getRenderPass(); }
		bool isFrameInProgress() const { return m_IsFrameStarted; }

		VkCommandBuffer GetCurrentCommandBuffer() const
		{
			assert(m_IsFrameStarted && "Cannot get command buffer when frame not in progress");
			return m_CommandBuffers[m_CurrentFrameIndex];
		}

		int GetFrameIndex() const
		{
			assert(m_IsFrameStarted && "Cannot get frame index when frame not in progress");
			return m_CurrentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void CreateCommandBuffer();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		CStellWindow& m_CStellWindow;
		CStellDevice& m_CStellDevice;
		std::unique_ptr<CStellSwapChain> m_CStellSwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentImageIndex;
		int m_CurrentFrameIndex;
		bool m_IsFrameStarted;
	};
}