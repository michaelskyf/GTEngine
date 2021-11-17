#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GTEngine/mesh.h>
#include <GTEngine/lib.h>
#include <GTEngine/output.h>

texture_t *texture_load(const char *path)
{
	texture_t *t = malloc(sizeof(texture_t));
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	if(data && t)
	{
		glGenTextures(1, &t->id);

		glBindTexture(GL_TEXTURE_2D, t->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	} else {
		LOGE("Failed to load texture %s", basename(path));
	}

	return t;
}
