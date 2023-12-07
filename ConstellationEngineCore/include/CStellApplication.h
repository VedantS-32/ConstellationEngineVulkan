#pragma once

#include "Minimal.h"
#include "CStellWindow.h"
#include "CStellShaderPipeline.h"
#include "CStellDevice.h"
#include "CStellSwapChain.h"
#include "CStellModel.h"

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

		int m_Width = 640;
		int m_Height = 640;

		void Run();

	private:
		void LoadModels();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffer();
		void FreeCommandBuffers();
		void DrawFrame();
		void RecreateSwapChain();
		void RecordCommandBuffer(int imageIndex);

		CStellWindow m_CStellWindow;
		CStellDevice m_CStellDevice;
		std::unique_ptr<CStellSwapChain> m_CStellSwapChain;
		std::unique_ptr<CStellModel> m_CStellModel;
		std::unique_ptr<CStellShaderPipeline> m_CStellShaderPipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}