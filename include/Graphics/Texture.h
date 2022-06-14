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
	void loadTexture(Texture *t, const char *path, TextureType type);
}

#endif