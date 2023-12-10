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
		struct Vertex
		{
			glm::vec3 m_position;
			glm::vec3 m_color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		CStellModel(CStellDevice& device, const std::vector<Vertex>& vertices);
		~CStellModel();

		CStellModel(const CStellModel&) = delete;
		CStellModel &operator=(const CStellModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		CStellDevice &m_CStellDevice;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		uint32_t m_vertexCount;
	};
}
