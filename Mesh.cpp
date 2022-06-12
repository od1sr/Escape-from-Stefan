#include "Mesh.h"
#include "glad/glad.h"
#include "logging.h"

using namespace sgl;
const int max_textures_num = 8;
std::vector<Texture> Mesh::no_textures;

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<Texture> &textures, std::vector<uint> &indices)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;
	setupMesh();
}

void Mesh::setupMesh()
{
	vao.loadVertexBuffer(&vertices[0], vertices.size() * sizeof(Vertex), GL_DYNAMIC_DRAW);
	vao.loadElementsBuffer(&indices[0], indices.size() * sizeof(uint), GL_DYNAMIC_DRAW);
	vao.setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	vao.setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_pos));
	vao.setVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	vao.setVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, diffuse_color));
	vao.setVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, specular_color));
}

void Mesh::draw(Shader &shader_program) const
{
	shader_program.use();
	vao.use();
	int specular_count = 0, diffuse_count = 0;
	int t_size = textures.size();
	for (int i = 0; (i < t_size) && (i < max_textures_num); ++i)
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + i)); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string name;
		switch (textures[i].type)
		{
		case TextureType::DIFFUSE:
			name = "diffuse_maps[" + std::to_string(diffuse_count++)+']';
			break;
		case TextureType::SPECULAR:
			name = "specular_maps[" + std::to_string(specular_count++)+']';
			break;
		}
		shader_program.setInt(("material." + name).c_str(), i);
		GL_CALL(glBindTexture(GL_TEXTURE_2D, textures[i].id));
	}
	shader_program.setInt("material.diff_textures_num", diffuse_count);
	shader_program.setInt("material.spec_textures_num", specular_count);
	shader_program.setInt("material.shininess", shininess);
	GL_CALL(glActiveTexture(GL_TEXTURE0));
	GL_CALL(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
}
