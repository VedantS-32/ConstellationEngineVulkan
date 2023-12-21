#include "CStellInput.h"

#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

namespace CStell
{
	void CStellInput::moveInPlaneXZ(CStellWindow& cstellWindow, float dt, CStellGameObject& gameObject)
	{
		auto window = cstellWindow.getGLFWwindow();
		auto width = cstellWindow.getWindowDimensions().x;
		auto height = cstellWindow.getWindowDimensions().y;
		glm::vec3 rotate{ 0 };
		if (glfwGetKey(window, m_keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
		if (glfwGetKey(window, m_keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
		if (glfwGetKey(window, m_keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
		if (glfwGetKey(window, m_keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;
		if (glfwGetKey(window, m_keys.rollClockwise) == GLFW_PRESS) rotate.z += 1.0f;
		if (glfwGetKey(window, m_keys.rollAntiClockwise) == GLFW_PRESS) rotate.z -= 1.0f;


		// Limit pitch values between about +/- 85ish degrees
		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject.transform.rotation.y;
		const glm::vec3 forwardDir{ sin(yaw), 0.0f, cos(yaw) };
		const glm::vec3 rightDir{ forwardDir.z, 0.0f, -forwardDir.x };
		const glm::vec3 upDir{ 0.0f, -1.0f, 0.0f };

		glm::vec3 moveDir{ 0.0f };
		if(glfwGetKey(window, m_keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if(glfwGetKey(window, m_keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if(glfwGetKey(window, m_keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if(glfwGetKey(window, m_keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
		if(glfwGetKey(window, m_keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if(glfwGetKey(window, m_keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
		{
			gameObject.transform.translation += m_moveSpeed * dt * glm::normalize(moveDir);
		}

		if (glfwGetMouseButton(window, m_keys.leftClick) == GLFW_PRESS)
		{
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			//glfwSetCursorPos(window, (width / 2), (height / 2));
		}

		else if (glfwGetMouseButton(window, m_keys.leftClick) == GLFW_RELEASE)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		{
			gameObject.transform.rotation += m_lookSpeed * dt * glm::normalize(rotate);
		}

	}

}
