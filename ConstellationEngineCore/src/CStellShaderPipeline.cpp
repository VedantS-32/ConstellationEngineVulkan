#include "CStellShaderPipeline.h"
#include "CStellModel.h"

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
        createGraphicsPipeline(configInfo, vertFilePath, fragFilePath);
    }

    CStellShaderPipeline::~CStellShaderPipeline()
    {
        vkDestroyShaderModule(m_CStellDevice.device(), m_vertShaderModule, nullptr);
        vkDestroyShaderModule(m_CStellDevice.device(), m_fragShaderModule, nullptr);
        vkDestroyPipeline(m_CStellDevice.device(), m_graphicsPipeline, nullptr);
    }

    void CStellShaderPipeline::bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
    }

    void CStellShaderPipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
    {
        configInfo.m_inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.m_inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.m_inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        configInfo.m_viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        configInfo.m_viewportInfo.viewportCount = 1;
        configInfo.m_viewportInfo.pViewports = nullptr;
        configInfo.m_viewportInfo.scissorCount = 1;
        configInfo.m_viewportInfo.pScissors = nullptr;

        configInfo.m_rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.m_rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.m_rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.m_rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.m_rasterizationInfo.lineWidth = 1.0f;
        configInfo.m_rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.m_rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.m_rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.m_rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.m_rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.m_rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

        configInfo.m_multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.m_multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.m_multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.m_multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.m_multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.m_multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.m_multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

        configInfo.m_colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        configInfo.m_colorBlendAttachment.blendEnable = VK_FALSE;
        configInfo.m_colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.m_colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.m_colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        configInfo.m_colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        configInfo.m_colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        configInfo.m_colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

        configInfo.m_colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        configInfo.m_colorBlendInfo.logicOpEnable = VK_FALSE;
        configInfo.m_colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        configInfo.m_colorBlendInfo.attachmentCount = 1;
        configInfo.m_colorBlendInfo.pAttachments = &configInfo.m_colorBlendAttachment;
        configInfo.m_colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        configInfo.m_colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        configInfo.m_colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        configInfo.m_colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        configInfo.m_depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.m_depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.m_depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.m_depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.m_depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.m_depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.m_depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.m_depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.m_depthStencilInfo.front = {};  // Optional
        configInfo.m_depthStencilInfo.back = {};   // Optional

        configInfo.m_dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        configInfo.m_dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        configInfo.m_dynamicStateInfo.pDynamicStates = configInfo.m_dynamicStateEnables.data();
        configInfo.m_dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.m_dynamicStateEnables.size());
        configInfo.m_dynamicStateInfo.flags = 0;
    }

    std::vector<char> CStellShaderPipeline::readFile(const std::string& filepath)
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

    void CStellShaderPipeline::createGraphicsPipeline(
        const PipelineConfigInfo& configInfo,
        const std::string vertFilePath,
        const std::string fragFilePath)
    {
        assert(configInfo.m_pipelineLayout != VK_NULL_HANDLE &&
            "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
        assert(configInfo.m_renderPass != VK_NULL_HANDLE &&
            "Cannot create graphics pipeline:: no renderPass provided in configInfo");

        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        createShaderModule(vertCode, &m_vertShaderModule);
        createShaderModule(fragCode, &m_fragShaderModule);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = m_vertShaderModule;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;
        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = m_fragShaderModule;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        auto bindingDescriptions = CStellModel::Vertex::getBindingDescriptions();
        auto attributeDescriptions = CStellModel::Vertex::getAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.m_inputAssemblyInfo;
        pipelineInfo.pViewportState = &configInfo.m_viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.m_rasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.m_multisampleInfo;

        pipelineInfo.pColorBlendState = &configInfo.m_colorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.m_depthStencilInfo;
        pipelineInfo.pDynamicState = &configInfo.m_dynamicStateInfo;

        pipelineInfo.layout = configInfo.m_pipelineLayout;
        pipelineInfo.renderPass = configInfo.m_renderPass;
        pipelineInfo.subpass = configInfo.m_subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_CStellDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline");
        }
    }
    void CStellShaderPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
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
