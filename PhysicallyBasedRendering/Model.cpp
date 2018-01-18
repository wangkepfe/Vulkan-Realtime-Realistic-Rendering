#include "VulkanApplicationPBR.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <unordered_map>

void VulkanApplicationPBR::loadModel() {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, MODEL_PATH.c_str())) {
		throw std::runtime_error(err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex = {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	computeTangentSpace();
}

void VulkanApplicationPBR::computeTangentSpace()
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		// Shortcuts for vertices
		const glm::vec3 & v0 = vertices[indices[i + 0]].pos;
		const glm::vec3 & v1 = vertices[indices[i + 1]].pos;
		const glm::vec3 & v2 = vertices[indices[i + 2]].pos;

		// Shortcuts for UVs
		const glm::vec2 & uv0 = vertices[indices[i + 0]].texCoord;
		const glm::vec2 & uv1 = vertices[indices[i + 1]].texCoord;
		const glm::vec2 & uv2 = vertices[indices[i + 2]].texCoord;

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		// calculation
		float r = deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x;
		if (r > 0.00001f || r < -0.00001f)
			r = 1.0f / r;
		else
			continue;

		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		// add
		vertices[indices[i + 0]].tangent += tangent;
		vertices[indices[i + 1]].tangent += tangent;
		vertices[indices[i + 2]].tangent += tangent;

		vertices[indices[i + 0]].bitangent += bitangent;
		vertices[indices[i + 1]].bitangent += bitangent;
		vertices[indices[i + 2]].bitangent += bitangent;
	}

	
	for(int i = 0; i < indices.size(); i++)
	{
		vertices[indices[i]].tangent = glm::normalize(vertices[indices[i]].tangent);
		vertices[indices[i]].bitangent = glm::normalize(vertices[indices[i]].tangent);
	}
}