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

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

	private:
		void InitWindow();

		const int m_Width = 800;
		const int m_Height = 800;

		std::string m_WindowName = "DefaultName";
		
		GLFWwindow* m_Window;
	};

}