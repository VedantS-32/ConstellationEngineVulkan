#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "Minimal.h"

namespace CStell
{
	class CStellWindow
	{
	public:
		CStellWindow();
		CStellWindow(int width , int height, std::string windowName);
		~CStellWindow();

		CStellWindow(const CStellWindow&) = delete;
		CStellWindow& operator=(const CStellWindow&) = delete;

		bool shouldClose();
		VkExtent2D getExtent() { return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) }; }
		bool wasWindowResized() { return m_frameBufferResized; }
		void resetWindowResizedFlag() { m_frameBufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return m_window; }
		
		glm::vec2 getWindowDimensions() { return glm::vec2(m_width, m_height); }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	private:
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
		void InitWindow();

		int m_width;
		int m_height;
		bool m_frameBufferResized = false;

		std::string m_windowName = "DefaultName";
		
		GLFWwindow* m_window;
	};

}