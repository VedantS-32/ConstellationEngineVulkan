#include "CStellApplication.h"
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
	CStellApplication::CStellApplication()
	{
		LoadGameObjects();
	}
	CStellApplication::~CStellApplication() {}

	void CStellApplication::Run()
	{
		CStellRenderSystem lCStellRenderSystem{ m_CStellDevice, m_CStellRenderer.GetSwapChainRenderPass() };

		while (!m_CStellWindow.ShouldClose())
		{
			glfwPollEvents();
			
			if (auto commandBuffer = m_CStellRenderer.BeginFrame())
			{
				m_CStellRenderer.BeginSwapChainRenderPass(commandBuffer);
				lCStellRenderSystem.RenderGameObjects(commandBuffer, m_GameObjects);
				m_CStellRenderer.EndSwapChainRenderPass(commandBuffer);
				m_CStellRenderer.EndFrame();
			}
		}

		vkDeviceWaitIdle(m_CStellDevice.device());
	}

	void CStellApplication::LoadGameObjects()
	{
		//std::vector<CStellModel::m_Vertex> vertices{ {{0.0f, -0.5}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}} };
		std::vector<CStellModel::m_Vertex> vertices{
			{{  0.0f, -0.5f }, {1.0f, 0.0f, 0.0f}},
			{{  0.5f,  0.5f }, {0.0f, 1.0f, 0.0f}},
			{{ -0.5f,  0.5f }, {0.0f, 0.0f, 1.0f}} };
		auto lCStellModel = std::make_shared<CStellModel>(m_CStellDevice, vertices);

		std::vector<glm::vec3> colors{
	  {1.f, .7f, .73f},
	  {1.f, .87f, .73f},
	  {1.f, 1.f, .73f},
	  {.73f, 1.f, .8f},
	  {.73, .88f, 1.f}  //
	};
		for (auto& color : colors) {
			color = glm::pow(color, glm::vec3{ 2.2f });
		}
		for (int i = 0; i < 40; i++) {
			auto triangle = CStellGameObject::CreateGameObject();
			triangle.model = lCStellModel;
			triangle.transform2D.scale = glm::vec2(.5f) + i * 0.025f;
			triangle.transform2D.rotation = i * glm::pi<float>() * .025f;
			triangle.color = colors[i % colors.size()];
			m_GameObjects.push_back(std::move(triangle));
		}
	}
}
