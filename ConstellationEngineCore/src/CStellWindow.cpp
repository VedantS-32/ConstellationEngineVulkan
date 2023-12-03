#include "CStellWindow.h"

#include <stdexcept>

namespace CStell
{
	CStellWindow::CStellWindow()
	{
		InitWindow();
	}
	CStellWindow::CStellWindow(int width, int height, std::string windowName) : m_Width(width), m_Height(height), m_WindowName(windowName)
	{
		InitWindow();
	}

	CStellWindow::~CStellWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool CStellWindow::ShouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void CStellWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);

	}
	void CStellWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}
}
