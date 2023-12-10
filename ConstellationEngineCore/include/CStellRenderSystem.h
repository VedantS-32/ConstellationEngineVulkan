#pragma once

#include "Minimal.h"
#include "CStellCamera.h"
#include "CStellShaderPipeline.h"
#include "CStellDevice.h"
#include "CStellGameObject.h"

#include <memory>
#include <vector>

namespace CStell
{
	class CStellRenderSystem
	{
	public:
		CStellRenderSystem(CStellDevice& device, VkRenderPass renderPass);
		~CStellRenderSystem();

		CStellRenderSystem(const CStellRenderSystem&) = delete;
		CStellRenderSystem& operator=(const CStellRenderSystem&) = delete;

		void renderGameObjects(
			VkCommandBuffer commandBuffer,
			std::vector<CStellGameObject> &gameObjects,
			const CStellCamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		CStellDevice& m_CStellDevice;

		std::unique_ptr<CStellShaderPipeline> m_CStellShaderPipeline;
		VkPipelineLayout m_pipelineLayout;
	};
}