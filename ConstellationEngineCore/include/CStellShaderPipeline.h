#pragma once

#include "CStellDevice.h"

#include <vector>

#include "Minimal.h"

namespace CStell
{
	struct PipelineConfigInfo 
	{
		VkPipelineViewportStateCreateInfo m_viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo m_rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo m_multisampleInfo;
		VkPipelineColorBlendAttachmentState m_colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo m_colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo m_depthStencilInfo;
		std::vector<VkDynamicState> m_dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo m_dynamicStateInfo; 
		VkPipelineLayout m_pipelineLayout = nullptr;
		VkRenderPass m_renderPass = nullptr;
		uint32_t m_subpass = 0;
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
		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
		
	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(
			const PipelineConfigInfo& configInfo,
			const std::string vertFilePath,
			const std::string fragFilePath);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		CStellDevice& m_CStellDevice;
		VkPipeline m_graphicsPipeline;
		VkShaderModule m_vertShaderModule;
		VkShaderModule m_fragShaderModule;
	};
}