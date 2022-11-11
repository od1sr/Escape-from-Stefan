#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "glad/glad.h"
#include "logging.h"
#include <string>

static uint getGlColorChannel(uint texture_nr_ch);

void sgl::loadTexture(Texture *t, const char *path, TextureType type, int *texture_size_buffer)
{
	stbi_set_flip_vertically_on_load(true);
	t->type = type;
	int width, height,
		nr_channels;
	unsigned char *texture = stbi_load(path, &width, &height, &nr_channels, 0);
	GL_CALL(glGenTextures(1, &(t->id)));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, t->id));
	if (texture)
	{
		if (texture_size_buffer)
		{
			texture_size_buffer[0] = width;
			texture_size_buffer[1] = height;
		}
		int gl_nr_ch = getGlColorChannel(nr_channels);
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, gl_nr_ch, width, height, 0, gl_nr_ch,
			GL_UNSIGNED_BYTE, texture));
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	}
	else
		save_error_log(std::string("Failed to load texture from ") + path);
	stbi_image_free(texture);
	GL_CALL(glEnable(GL_DEPTH_TEST));
}

static uint getGlColorChannel(uint texture_nr_ch)
{
	switch (texture_nr_ch)
	{
	case 1:
		return GL_RED;
		break;
	case 3:
		return GL_RGB;
		break;
	case 4:
		return GL_RGBA;
		break;
	default:
		return -1;
		break;
	}
}