#include "mesh.h"
namespace Hub
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		setupMesh();
	}

	void Mesh::draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
			}
			else if(name == "texture_specular")
			{
				number = std::to_string(specularNr++);
			}
			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(*VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNALED, 0);
		glBindVertexArray(0);
	}

	void Mesh::setupMesh()
	{
		VAO = VertexArray::create();
		VBO = VertexBuffer::create(&vertices[0], vertices.size() * sizeof(Vertex), BufferUsage::StaticDraw);
		EBO = ElementBuffer::create(&indices[0], indices.size() * sizeof(unsigned int), BufferUsage::StaticDraw);
		VAO->bindElements(*EBO);
		VAO->bindAttribute(0, 3, *VBO, Type::Float, sizeof(Vertex), offsetof(Vertex, position));
		VAO->bindAttribute(1, 3, *VBO, Type::Float, sizeof(Vertex), offsetof(Vertex, normal));
		VAO->bindAttribute(2, 2, *VBO, Type::Float, sizeof(Vertex), offsetof(Vertex, texCoords));
		glBindVertexArray(0);
	}

}