#include "CStellShaderPipeline.h"

#include <fstream>
#include <stdexcept>
#include <cassert>

namespace CStell
{
    CStellShaderPipeline::CStellShaderPipeline(
        CStellDevice& device,
        const PipelineConfigInfo& configInfo,
        const std::string vertFilePath,
        const std::string fragFilePath):
        m_CStellDevice(device)
    {
        CreateGraphicsPipeline(configInfo, vertFilePath, fragFilePath);
    }

    CStellShaderPipeline::~CStellShaderPipeline()
    {
        vkDestroyShaderModule(m_CStellDevice.device(), m_VertShaderModule, nullptr);
        vkDestroyShaderModule(m_CStellDevice.device(), m_FragShaderModule, nullptr);
        vkDestroyPipeline(m_CStellDevice.device(), m_GraphicsPipeline, nullptr);
    }

    void CStellShaderPipeline::bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
    }

    PipelineConfigInfo CStellShaderPipeline::m_DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};
        configInfo.m_InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.m_InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.m_InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;


        configInfo.m_Viewport.x = 0.0f;
        configInfo.m_Viewport.y = 0.0f;
        configInfo.m_Viewport.width = static_cast<float>(width);
        configInfo.m_Viewport.height = static_cast<float>(height);
        configInfo.m_Viewport.minDepth = 0.0f;
        configInfo.m_Viewport.maxDepth = 1.0f;

        configInfo.m_Scissor.offset = { 0, 0 };
        configInfo.m_Scissor.extent = { width, height };

        configInfo.m_RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.m_RasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.m_RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.m_RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.m_RasterizationInfo.lineWidth = 1.0f;
        configInfo.m_RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.m_RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.m_RasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.m_RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.m_RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.m_RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

        configInfo.m_MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.m_MultisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.m_MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.m_MultisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.m_MultisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.m_MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.m_MultisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        configInfo.m_ColorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        configInfo.m_ColorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.m_ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.m_ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.m_ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.m_ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.m_ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.m_ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

        configInfo.m_ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.m_ColorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.m_ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.m_ColorBlendInfo.attachmentCount = 1;
        configInfo.m_ColorBlendInfo.pAttachments = &configInfo.m_ColorBlendAttachment;
        configInfo.m_ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.m_ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.m_ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.m_ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        configInfo.m_DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.m_DepthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.m_DepthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.m_DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.m_DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.m_DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.m_DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.m_DepthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.m_DepthStencilInfo.front = {};  // Optional
        configInfo.m_DepthStencilInfo.back = {};   // Optional

        return configInfo;
    }

    std::vector<char> CStellShaderPipeline::ReadFile(const std::string& filepath)
    {
        std::ifstream file{ filepath, std::ios::ate | std::ios::binary };

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void CStellShaderPipeline::CreateGraphicsPipeline(
        const PipelineConfigInfo& configInfo,
        const std::string vertFilePath,
        const std::string fragFilePath)
    {
        assert(configInfo.m_PipelineLayout != VK_NULL_HANDLE &&
            "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
        assert(configInfo.m_RenderPass != VK_NULL_HANDLE &&
            "Cannot create graphics pipeline:: no renderPass provided in configInfo");

        auto vertCode = ReadFile(vertFilePath);
        auto fragCode = ReadFile(fragFilePath);

        CreateShaderModule(vertCode, &m_VertShaderModule);
        CreateShaderModule(fragCode, &m_FragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = m_VertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = m_FragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &configInfo.m_Viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &configInfo.m_Scissor;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.m_InputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.m_RasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.m_MultisampleInfo;

        pipelineInfo.pColorBlendState = &configInfo.m_ColorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.m_DepthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = configInfo.m_PipelineLayout;
        pipelineInfo.renderPass = configInfo.m_RenderPass;
        pipelineInfo.subpass = configInfo.m_Subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_CStellDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline");
        }
    }
    void CStellShaderPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(m_CStellDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }
}
