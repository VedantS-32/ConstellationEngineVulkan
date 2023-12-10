#include "CStellApplication.h"

#include "CStellCamera.h"
#include "CStellRenderSystem.h"
#include "CStellInput.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <cassert>
#include <stdexcept>

namespace CStell
{
	CStellApplication::CStellApplication()
	{
		loadGameObjects();
	}
	CStellApplication::~CStellApplication() {}

	void CStellApplication::run()
	{
		CStellRenderSystem lCStellRenderSystem{ m_CStellDevice, m_CStellRenderer.getSwapChainRenderPass() };
		CStellCamera camera{};
		camera.setViewTarget(glm::vec3(-1.0f, -1.2f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

		auto viewerObject = CStellGameObject::createGameObject();
		CStellInput cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_CStellWindow.shouldClose())
		{ 
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			//frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			cameraController.moveInPlaneXZ(m_CStellWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = m_CStellRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.01f, 10.0f);
			
			if (auto commandBuffer = m_CStellRenderer.beginFrame())
			{
				m_CStellRenderer.beginSwapChainRenderPass(commandBuffer);
				lCStellRenderSystem.renderGameObjects(commandBuffer, m_gameObjects, camera);
				m_CStellRenderer.endSwapChainRenderPass(commandBuffer);
				m_CStellRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(m_CStellDevice.device());
	}

	// temporary helper function, creates a 1x1x1 cube centered at offset
	std::unique_ptr<CStellModel> createCubeModel(CStellDevice& device, glm::vec3 offset) {
		std::vector<CStellModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : vertices) {
			v.m_position += offset;
		}
		return std::make_unique<CStellModel>(device, vertices);
	}

	void CStellApplication::loadGameObjects()
	{
		std::shared_ptr<CStellModel> lCStellModel = createCubeModel(m_CStellDevice, { 0.0f, 0.0f, 0.0f });

		auto cube = CStellGameObject::createGameObject();
		cube.model = lCStellModel;
		cube.transform.translation = { 0.0f, 0.0f, 2.5f };
		cube.transform.scale = { 0.5f, 0.5f, 0.5f };
		m_gameObjects.push_back(std::move(cube));
	}
}
