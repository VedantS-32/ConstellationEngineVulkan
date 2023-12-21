#pragma once

#include "CStellGameObject.h"
#include "CStellWindow.h"

namespace CStell
{
	class CStellInput
	{
	public:
		struct KeyMappings
		{
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
            int leftClick = GLFW_MOUSE_BUTTON_LEFT;
            int rightClick = GLFW_MOUSE_BUTTON_RIGHT;
            //int scrollUp = GLFW_MOUSE_BUTTON_MIDDLE;
            int rollClockwise = GLFW_KEY_1;
            int rollAntiClockwise = GLFW_KEY_2;
		};

        void moveInPlaneXZ(CStellWindow& cstellWindow, float dt, CStellGameObject& gameObject);

        KeyMappings m_keys{};
        float m_moveSpeed{ 3.0f };
        float m_lookSpeed{ 1.5f };
	};
}