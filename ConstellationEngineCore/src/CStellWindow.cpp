#include "CStellWindow.h"

#include <stdexcept>

namespace CStell
{
	CStellWindow::CStellWindow()
	{
		InitWindow();
	}
	CStellWindow::CStellWindow(int width, int height, std::string windowName) : m_width(width), m_height(height), m_windowName(windowName)
	{
		InitWindow();
	}

	CStellWindow::~CStellWindow()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool CStellWindow::shouldClose()
	{
		return glfwWindowshouldClose(m_window);
	}

	void CStellWindow::FrameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto lCStellWindow = reinterpret_cast<CStellWindow*>(glfwGetWindowUserPointer(window));
		lCStellWindow->m_frameBufferResized = true;
		lCStellWindow->m_width = width;
		lCStellWindow->m_height = height;
	}

	void CStellWindow::InitWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, FrameBufferResizeCallback);

	}
	void CStellWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}
}
