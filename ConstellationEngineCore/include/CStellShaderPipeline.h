#pragma once

#include "CStellDevice.h"

#include <vector>

#include "Minimal.h"

namespace CStell
{
	struct PipelineConfigInfo 
	{
		VkPipelineViewportStateCreateInfo m_ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo m_InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo m_RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo m_MultisampleInfo;
		VkPipelineColorBlendAttachmentState m_ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo m_ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo m_DepthStencilInfo;
		std::vector<VkDynamicState> m_DynamicStateEnables;
		VkPipelineDynamicStateCreateInfo m_DynamicStateInfo; 
		VkPipelineLayout m_PipelineLayout = nullptr;
		VkRenderPass m_RenderPass = nullptr;
		uint32_t m_Subpass = 0;
	};

	class CStellShaderPipeline
	{
	public:
		CStellShaderPipeline() = default;
		CStellShaderPipeline(
			CStellDevice& device,
			const PipelineConfigInfo& configInfo,
			const std::string vertFilePath,
			const std::string fragFilePath);

		~CStellShaderPipeline();

		CStellShaderPipeline(const CStellShaderPipeline&) = delete;
		CStellShaderPipeline& operator=(const CStellShaderPipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		static void m_DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
		
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