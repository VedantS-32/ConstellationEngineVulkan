#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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
		bool WasWindowResized() { return m_frameBufferResized; }
		void ResetWindowResizedFlag() { m_frameBufferResized = false; }
		GLFWwindow* getGLFWwindow() const { return m_window; }

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