#pragma once

#include "CStellModel.h"

#include <memory>

namespace CStell
{
	struct CTranform2DComponent
	{
		glm::vec2 translation{}; //Position Offset
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation;
		
		glm::mat2 mat2()
		{ 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c, s}, {-s, c} };

			glm::mat2 scaleMatrix{ {scale.x, 0.0f}, {0.0f, scale.y} }; //Takes Columns
			return rotMatrix * scaleMatrix;
		}
	};

	class CStellGameObject
	{
	public:
		using id_t = unsigned int;

		static CStellGameObject CreateGameObject()
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
		CTranform2DComponent transform2D{};

	private:
		CStellGameObject(id_t objID) : ID(objID) {}

		id_t ID;
	};
}