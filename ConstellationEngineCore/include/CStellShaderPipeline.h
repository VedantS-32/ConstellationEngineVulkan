#pragma once

#include "CStellDevice.h"

#include <vector>

#include "Minimal.h"

namespace CStell
{
	struct PipelineConfigInfo 
	{
		VkViewport m_Viewport;
		VkRect2D m_Scissor;
		VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo m_RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo m_MultisampleInfo;
		VkPipelineColorBlendAttachmentState m_ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo m_ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo m_DepthStencilInfo;
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkRenderPass m_RenderPass = nullptr;
		uint32_t m_Subpass = 0;
	};

	class CStellShaderPipeline
	{
	public:
		CStellShaderPipeline(
			CStellDevice& device,
			const PipelineConfigInfo& configInfo,
			const std::string vertFilePath,
			const std::string fragFilePath);

		~CStellShaderPipeline();

		CStellShaderPipeline(const CStellShaderPipeline&) = delete;
		void operator=(const CStellShaderPipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		static PipelineConfigInfo m_DefaultPipelineConfigInfo(uint32_t width, uint32_t height);
		
	private:
		static std::vector<char> ReadFile(const std::string& filePath);

		void CreateGraphicsPipeline(
			const PipelineConfigInfo& configInfo,
			const std::string vertFilePath,
			const std::string fragFilePath);

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		CStellDevice& m_CStellDevice;
		VkPipeline m_GraphicsPipeline;
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;
	};
}