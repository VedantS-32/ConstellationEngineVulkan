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
		glm::mat4 transform{ 1.0f };
		glm::mat4 normalMatrix{ 1.0f };
	};

	CStellRenderSystem::CStellRenderSystem(CStellDevice& device, VkRenderPass renderPass) :
		m_CStellDevice(device)
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}
	CStellRenderSystem::~CStellRenderSystem()
	{
		vkDestroyPipelineLayout(m_CStellDevice.device(), m_pipelineLayout, nullptr);
	}

	void CStellRenderSystem::createPipelineLayout()
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
		if (vkCreatePipelineLayout(m_CStellDevice.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void CStellRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		CStellShaderPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.m_renderPass = renderPass;
		pipelineConfig.m_pipelineLayout = m_pipelineLayout;
		m_CStellShaderPipeline = std::make_unique<CStellShaderPipeline>(
			m_CStellDevice,
			pipelineConfig,
			"shader/vert.spv",
			"shader/frag.spv"
		);
	}

	void CStellRenderSystem::renderGameObjects(
		VkCommandBuffer commandBuffer,
		std::vector<CStellGameObject>& gameObjects,
		const CStellCamera& camera)
	{
		m_CStellShaderPipeline->bind(commandBuffer);

		auto projectionView = camera.getProjection() * camera.getView();

		for (auto& obj : gameObjects)
		{
			SimplePushConstantData push{};
			auto modelMatrix = obj.transform.mat4();
			push.transform = projectionView * modelMatrix;
			push.normalMatrix = obj.transform.normalMatrix();;

			vkCmdPushConstants(
				commandBuffer,
				m_pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);
			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);
		}
	}
}
