#ifndef _MODEL_CLASS_H_
#define _MODEL_CLASS_H_

#include "Mesh.h"
#include <vector>
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include <string>

namespace sgl
{
	class Model
	{
	private:
		std::vector<Mesh*> meshes;
		std::string model_dir;

		bool checkLoadingErrors(const aiScene *scene, const Assimp::Importer &importer) const;
		void processNode(aiNode *node, const aiScene *scene);
		void processMesh(aiMesh *mesh, const aiScene *scene);
		void loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType t, std::vector<Texture>& textures) const;
	public:
		Model(const char *model_path);
		Model();
		void loadFromFile(const char *model_path);
		void draw(Shader &shader_program) const;
		~Model();
	};
}
#endif // !_MODEL_CLASS_H_