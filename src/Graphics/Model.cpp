#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "logging.h"
#include <string>
#include <vector>
#include "Texture.h"
#include <map>
#include <algorithm>
#include <string.h>

using namespace sgl;
static const char *current_texture_path_to_found_g;

Model::Model(const char *model_path)
{	
	loadFromFile(model_path);
}

Model::Model()
{
}

void Model::loadFromFile(const char* model_path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate);
	if (!checkLoadingErrors(scene, importer))
		return;
	model_dir = model_path;
	model_dir = model_dir.substr(0, model_dir.find_last_of('\\')).c_str();
	processNode(scene->mRootNode, scene);
}

bool sgl::Model::checkLoadingErrors(const aiScene *scene, const Assimp::Importer &importer) const
{
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		save_error_log((std::string("Failed to load model: ") + importer.GetErrorString()).c_str()); 
		return false;
	};
	return true;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	// ќбрабатываем все меши (если они есть) у выбранного узла
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	// » проделываем то же самое дл€ всех дочерних узлов
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	std::vector<Texture> textures;
	aiMaterial *material = NULL;
	float shininess = 0.f;
	if (mesh->mMaterialIndex >= 0)
		material = scene->mMaterials[mesh->mMaterialIndex];

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// ќбрабатываем координаты вершин, нормали и текстурные координаты
		vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		if (mesh->mTextureCoords[0])
			vertex.texture_pos = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		else
			vertex.texture_pos = { 0.f, 0.f };
			/*vertex.diffuse_color = vec4(
				mesh->mColors[0][i], mesh->mColors[1][i], mesh->mColors[2][i], mesh->mColors[3][i]);*/
		if (material)
		{
			aiColor4D diffuse, specular;
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
				vertex.diffuse_color = vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			else
				vertex.diffuse_color = vec4(0.f, 0.f, 0.f, 0.0f);
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &specular))
				vertex.specular_color = vec4(specular.r, specular.g, specular.b, specular.a);
			else
				vertex.specular_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	// ќбрабатываем индексы
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// ќбрабатываем материал
	if(material)
	{
		std::vector<Texture> diffuse_maps, specular_maps;
		loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, diffuse_maps);
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR, specular_maps);
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
		material->Get(AI_MATKEY_SHININESS, shininess);
		
	}
	Mesh *m = new Mesh(vertices, textures, indices);
	m->shininess = (int)shininess;
	meshes.push_back(m);
}

void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType t, std::vector<Texture> &textures) const
{
	static std::map<const char*, Texture>  loaded_textures;
	int texture_count = mat->GetTextureCount(type);

	for (unsigned int i = 0; i < texture_count; i++)
	{
		aiString str;
		std::string texture_path;
		mat->GetTexture(type, i, &str);
		texture_path = model_dir + "\\" + std::string(str.C_Str());
		current_texture_path_to_found_g = texture_path.c_str();
		std::map<const char*, Texture>::iterator l_t_end = loaded_textures.end();
		std::map<const char*, Texture>::iterator tex = std::find_if(loaded_textures.begin(), l_t_end,
			[](std::pair<const char*, Texture> t) {
				return strcmp(t.first, current_texture_path_to_found_g) == 0;
			}
		);
		if (tex != l_t_end)
			textures.push_back(tex->second);
		else
		{
			Texture texture;
			loadTexture(&texture, texture_path.c_str(), t);
			textures.push_back(texture);
			loaded_textures[current_texture_path_to_found_g] = texture;
		}
	}
}


void Model::draw(Shader &shader_program) const
{
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i]->draw(shader_program);
}

Model::~Model()
{
	while (meshes.size())
	{
		delete meshes.back();
		meshes.pop_back();
	}
}
