#ifndef _MESH_H_
#define _MESH_H_

#include "glm/glm.hpp"
#include <vector>
#include "VAO.h"
#include "Shader.h"
#include "Texture.h"

typedef unsigned int uint;

using glm::vec3;
using glm::vec2;
using glm::vec4;
namespace sgl
{
	struct Vertex
	{
		vec3 pos;
		vec2 texture_pos;
		vec3 normal;
		vec4 diffuse_color;
		vec4 specular_color;
	};

	class Mesh
	{
	private:
		static std::vector<Texture> no_textures;

		std::vector<Vertex> vertices;
		std::vector<Texture> textures;
		std::vector<uint> indices;
		VAO vao;

		void setupMesh();

	public:
		Mesh(std::vector<Vertex> &vertices, std::vector<Texture> &textures, std::vector<uint> &indices);
		void draw(Shader &shader_program) const;

		int shininess;

	};
}

#endif // !_MESH_H_