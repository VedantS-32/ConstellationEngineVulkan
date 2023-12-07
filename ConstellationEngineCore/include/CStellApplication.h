#pragma once

#include "Minimal.h"
#include "CStellWindow.h"
#include "CStellShaderPipeline.h"
#include "CStellDevice.h"
#include "CStellSwapChain.h"
#include "CStellGameObject.h"

#include <memory>
#include <vector>

namespace CStell
{
	class CStellApplication
	{
	public:
		CStellApplication();
		~CStellApplication();

		CStellApplication(const CStellApplication&) = delete;
		CStellApplication& operator=(const CStellApplication&) = delete;

		int m_Width = 800;
		int m_Height = 600;

		void Run();

	private:
		void LoadGameObjects();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffer();
		void FreeCommandBuffers();
		void DrawFrame();
		void RecreateSwapChain();
		void RecordCommandBuffer(int imageIndex);
		void RenderGameObjects(VkCommandBuffer commandBuffer);

		CStellWindow m_CStellWindow;
		CStellDevice m_CStellDevice;
		std::unique_ptr<CStellSwapChain> m_CStellSwapChain;
		std::vector<CStellGameObject> m_GameObjects;
		std::unique_ptr<CStellShaderPipeline> m_CStellShaderPipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}