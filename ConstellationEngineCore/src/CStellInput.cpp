#include "CStellInput.h"

namespace CStell
{
	void CStellInput::moveInPlaneXZ(GLFWwindow* window, float dt, CStellGameObject& gameObject)
	{
		glm::vec3 rotate{ 0 };
		if (glfwGetKey(window, m_keys.lookRight) == GLFW_PRESS) rotate.y += 1.0f;
		if (glfwGetKey(window, m_keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0f;
		if (glfwGetKey(window, m_keys.lookUp) == GLFW_PRESS) rotate.x += 1.0f;
		if (glfwGetKey(window, m_keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		{
			gameObject.transform.rotation += m_lookSpeed * dt * glm::normalize(rotate);
		}

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

	}

}
