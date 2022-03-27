#pragma once
#include "gmath.h"
#include "shader.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "element_buffer.h"
#include <string>
#include <vector>

namespace Hub
{
	namespace MeshData
	{
		struct Vertex
		{
			Vector3 position;
			Vector3 normal;
			Vector2 texCoords;
		};

		struct Texture
		{
			unsigned int id;
			std::string type;
		};
	}

	using namespace MeshData;
	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

		void draw(Shader& shader);

	private:
		SPVertexArray VAO;
		SPVertexBuffer VBO;
		SPElementBuffer EBO;

		void setupMesh();
		
	};
}