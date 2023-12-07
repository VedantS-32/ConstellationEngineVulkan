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

		bool ShouldClose();
		VkExtent2D getExtent() { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }
		bool WasWindowResized() { return m_FrameBufferResized; }
		void ResetWindowResizedFlag() { m_FrameBufferResized = false; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

	private:
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
		void InitWindow();

		int m_Width;
		int m_Height;
		bool m_FrameBufferResized = false;

		std::string m_WindowName = "DefaultName";
		
		GLFWwindow* m_Window;
	};

}