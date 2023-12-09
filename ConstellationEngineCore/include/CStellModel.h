#pragma once

#include "CStellDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace CStell
{
	class CStellModel
	{
	public:
		struct m_Vertex
		{
			glm::vec3 m_Position;
			glm::vec3 m_Color;

			static std::vector<VkVertexInputBindingDescription> m_GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> m_GetAttributeDescriptions();
		};

		CStellModel(CStellDevice& device, const std::vector<m_Vertex>& vertices);
		~CStellModel();

		CStellModel(const CStellModel&) = delete;
		CStellModel &operator=(const CStellModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void CreateVertexBuffers(const std::vector<m_Vertex>& vertices);

		CStellDevice &m_CStellDevice;
		VkBuffer m_VextexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32_t m_VertexCount;
	};
}
