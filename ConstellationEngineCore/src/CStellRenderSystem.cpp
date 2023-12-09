#include "CStellRenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <cassert>
#include <stdexcept>

namespace CStell
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{ 1.0f };
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	CStellRenderSystem::CStellRenderSystem(CStellDevice& device, VkRenderPass renderPass) :
		m_CStellDevice(device)
	{
		CreatePipelineLayout();
		CreatePipeline(renderPass);
	}
	CStellRenderSystem::~CStellRenderSystem()
	{
		vkDestroyPipelineLayout(m_CStellDevice.device(), m_PipelineLayout, nullptr);
	}

	void CStellRenderSystem::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_CStellDevice.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void CStellRenderSystem::CreatePipeline(VkRenderPass renderPass)
	{
		assert(m_PipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		CStellShaderPipeline::m_DefaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.m_RenderPass = renderPass;
		pipelineConfig.m_PipelineLayout = m_PipelineLayout;
		m_CStellShaderPipeline = std::make_unique<CStellShaderPipeline>(
			m_CStellDevice,
			pipelineConfig,
			"shader/vert.spv",
			"shader/frag.spv"
		);
	}

	void CStellRenderSystem::RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<CStellGameObject>& gameObjects)
	{
		int i = 0;
		for (auto& obj : gameObjects)
		{
			i += 1;
			obj.transform2D.rotation =
				glm::mod<float>(obj.transform2D.rotation + 0.0001f * i, 2.f * glm::pi<float>());
		}

		m_CStellShaderPipeline->bind(commandBuffer);
		for (auto& obj : gameObjects)
		{
			SimplePushConstantData push{};
			push.offset = obj.transform2D.translation;
			push.color = obj.color;
			push.transform = obj.transform2D.mat2();

			vkCmdPushConstants(
				commandBuffer,
				m_PipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}
}
