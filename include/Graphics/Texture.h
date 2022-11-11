#ifndef _TEXUTRE_STRUCT_H_
#define _TEXUTRE_STRUCT_H_

typedef unsigned int uint;
namespace sgl
{
	enum class TextureType { DIFFUSE, SPECULAR, UNINITIALIZED };

	struct Texture
	{
		uint id;
		TextureType type;
	};
	// texture_size_buffer is buffer which contatins 2 parameters: width and height of picture
	void loadTexture(Texture *t, const char *path, TextureType type, int *texture_size_buffer = nullptr);
}

#endif