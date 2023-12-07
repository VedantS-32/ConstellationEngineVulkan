#include "CStellModel.h"

#include <cassert>

namespace CStell
{
	CStellModel::CStellModel(CStellDevice& device, const std::vector<m_Vertex>& vertices) : m_CStellDevice(device)
	{
		CreateVertexBuffers(vertices);
	}

	CStellModel::~CStellModel()
	{
		vkDestroyBuffer(m_CStellDevice.device(), m_VextexBuffer, nullptr);
		vkFreeMemory(m_CStellDevice.device(), m_VertexBufferMemory, nullptr);
	}

	void CStellModel::bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_VextexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void CStellModel::draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	}

	void CStellModel::CreateVertexBuffers(const std::vector<m_Vertex>& vertices)
	{
		m_VertexCount = static_cast<uint32_t>(vertices.size());
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
		m_CStellDevice.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_VextexBuffer,
			m_VertexBufferMemory
		);

		void* data;
		vkMapMemory(m_CStellDevice.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_CStellDevice.device(), m_VertexBufferMemory);
	}
	std::vector<VkVertexInputBindingDescription> CStellModel::m_Vertex::m_GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(m_Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}
	std::vector<VkVertexInputAttributeDescription> CStellModel::m_Vertex::m_GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(m_Vertex, m_Position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(m_Vertex, m_Color);
		return attributeDescriptions;
	}
}
