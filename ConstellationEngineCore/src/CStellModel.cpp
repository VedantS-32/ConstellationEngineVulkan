#include "CStellModel.h"
#include "CStellUtils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <cassert>
#include <unordered_map>

namespace std
{
	template <>
	struct hash<CStell::CStellModel::Vertex>
	{
		size_t operator()(CStell::CStellModel::Vertex const& vertex) const
		{
			size_t seed = 0;
			CStell::hashCombine(seed, vertex.m_position, vertex.m_color, vertex.m_normal, vertex.m_uv);
			return seed;
		}
	};
}

namespace CStell
{
	CStellModel::CStellModel(CStellDevice& device, const CStellModel::Builder& builder) : m_CStellDevice(device)
	{
		createVertexBuffers(builder.m_vertices);
		createIndexBuffers(builder.m_indices);
	}

	CStellModel::~CStellModel()
	{
		vkDestroyBuffer(m_CStellDevice.device(), m_vertexBuffer, nullptr);
		vkFreeMemory(m_CStellDevice.device(), m_vertexBufferMemory, nullptr);

		if (m_hasIndexBuffer)
		{
			vkDestroyBuffer(m_CStellDevice.device(), m_indexBuffer, nullptr);
			vkFreeMemory(m_CStellDevice.device(), m_indexBufferMemory, nullptr);
		}
	}

	std::unique_ptr<CStellModel> CStellModel::createModelFromFile(CStellDevice& device, const std::string& filepath)
	{
		Builder builder{};
		builder.loadModel(filepath);
		console_log("Vertex Count: " << builder.m_vertices.size());
		return std::make_unique<CStellModel>(device, builder);
	}

	void CStellModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (m_hasIndexBuffer) {
			vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void CStellModel::draw(VkCommandBuffer commandBuffer)
	{
		if (m_hasIndexBuffer) { vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0); }
		else { vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0); }
	}

	void CStellModel::createVertexBuffers(const std::vector<Vertex>& vertices)
	{
		m_vertexCount = static_cast<uint32_t>(vertices.size());
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		m_CStellDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(m_CStellDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_CStellDevice.device(), stagingBufferMemory);

		m_CStellDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_vertexBuffer,
			m_vertexBufferMemory
		);

		m_CStellDevice.copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

		vkDestroyBuffer(m_CStellDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(m_CStellDevice.device(), stagingBufferMemory, nullptr);

	}

	void CStellModel::createIndexBuffers(const std::vector<uint32_t>& indices)
	{
		m_indexCount = static_cast<uint32_t>(indices.size());
		m_hasIndexBuffer = m_indexCount > 0;

		if (!m_hasIndexBuffer) { return; }

		VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		m_CStellDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(m_CStellDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_CStellDevice.device(), stagingBufferMemory);

		m_CStellDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_indexBuffer,
			m_indexBufferMemory
		);

		m_CStellDevice.copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

		vkDestroyBuffer(m_CStellDevice.device(), stagingBuffer, nullptr);
		vkFreeMemory(m_CStellDevice.device(), stagingBufferMemory, nullptr);
	}

	std::vector<VkVertexInputBindingDescription> CStellModel::Vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> CStellModel::Vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, m_position)});
		attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, m_color)});
		attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, m_normal)});
		attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, m_uv)});

		return attributeDescriptions;
	}
	void CStellModel::Builder::loadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		m_vertices.clear();
		m_indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};
				
				if (index.vertex_index >= 0)
				{
					vertex.m_position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.m_color = {
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2]
					};

				}
				if (index.normal_index >= 0)
				{
					vertex.m_normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};
				}
				if (index.texcoord_index >= 0)
				{
					vertex.m_uv = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
					m_vertices.push_back(vertex);
				}
				m_indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}
