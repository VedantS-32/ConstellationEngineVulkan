#pragma once

#include "Minimal.h"
#include "CStellWindow.h"
#include "CStellShaderPipeline.h"
#include "CStellDevice.h"
#include "CStellSwapChain.h"

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

		int m_Width = 1280;
		int m_Height = 720;

		void Run();

	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffer();
		void DrawFrame();

		CStellWindow m_CStellWindow;
		CStellDevice m_CStellDevice;
		CStellSwapChain m_CStellSwapChain;
		std::unique_ptr<CStellShaderPipeline> m_CStellShaderPipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};
}