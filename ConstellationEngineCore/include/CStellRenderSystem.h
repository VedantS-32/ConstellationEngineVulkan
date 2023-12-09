#pragma once

#include "Minimal.h"
#include "CStellWindow.h"
#include "CStellShaderPipeline.h"
#include "CStellDevice.h"
#include "CStellRenderer.h"
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

		void RenderGameObjects(VkCommandBuffer commandBuffer, std::vector<CStellGameObject> &gameObjects);

	private:
		void CreatePipelineLayout();
		void CreatePipeline(VkRenderPass renderPass);

		CStellDevice& m_CStellDevice;

		std::unique_ptr<CStellShaderPipeline> m_CStellShaderPipeline;
		VkPipelineLayout m_PipelineLayout;
	};
}