#pragma once

#include "CStellModel.h"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace CStell
{
	struct CTranformComponent
	{
		glm::vec3 translation{}; //Position Offset
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation{};
		
		// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
		// Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
		// https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
		glm::mat4 mat4();
		glm::mat4 normalMatrix();
	};

	class CStellGameObject
	{
	public:
		using id_t = unsigned int;

		static CStellGameObject createGameObject()
		{
			static id_t CurrentID = 0;
			return CStellGameObject(CurrentID++);
		}

		CStellGameObject(const CStellGameObject&) = delete;
		CStellGameObject& operator=(const CStellGameObject&) = delete;
		CStellGameObject(CStellGameObject&&) = default;
		CStellGameObject& operator=(CStellGameObject&&) = delete;

		const id_t GetID() { return ID; }

		std::shared_ptr<CStellModel> model{};
		glm::vec3 color{};
		CTranformComponent transform{};

	private:
		CStellGameObject(id_t objID) : ID(objID) {}

		id_t ID;
	};
}