#pragma once

#include "CStellDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>

namespace CStell
{
	class CStellModel
	{
	public:
		struct Vertex
		{
			glm::vec3 m_position;
			glm::vec3 m_color;
			glm::vec3 m_normal;
			glm::vec2 m_uv;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const
			{
				return m_position == other.m_position && m_color == other.m_color && m_normal == other.m_normal &&
					m_uv == other.m_uv;
			}
		};

		struct Builder
		{
			std::vector<Vertex> m_vertices{};
			std::vector<uint32_t> m_indices{};

			void loadModel(const std::string& filepath);
		};

		CStellModel(CStellDevice& device, const CStellModel::Builder &builder);
		~CStellModel();

		CStellModel(const CStellModel&) = delete;
		CStellModel &operator=(const CStellModel&) = delete;

		static std::unique_ptr<CStellModel> createModelFromFile(
			CStellDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		CStellDevice &m_CStellDevice;

		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffers(const std::vector<uint32_t>& indices);

		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		uint32_t m_vertexCount;

		bool m_hasIndexBuffer = false;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;
		uint32_t m_indexCount;
	};
}
