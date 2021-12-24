/*
    This file is part of GTEngine.

    GTEngine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTEngine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTEngine. If not, see <https://www.gnu.org/licenses/>.
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GTEngine/texture.h>
#include <GTEngine/vector.h>
#include <GTEngine/engine.h>
#include <GTEngine/output.h>

static texture_t *texture_create(unsigned char *data, size_t width, size_t height, int channels);

texture_t *texture_load(const char *path)
{
	stbi_set_flip_vertically_on_load(1);
	texture_t *t = NULL;
	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

	if(data)
	{
		t = texture_create(data, width, height, channels);
		stbi_image_free(data);
	}
	return t;
}

texture_t *texture_load_memory(const unsigned char *img, size_t w, size_t h)
{
	texture_t *t = NULL;
	int width, height, channels;
	unsigned char *data = stbi_load_from_memory(img, w * h, &width, &height, &channels, 0);

	if(data)
	{
		t = texture_create(data, width, height, channels);
		stbi_image_free(data);
	}
	return t;
}

static texture_t *texture_create(unsigned char *data, size_t width, size_t height, int channels)
{
		int index = vector_push_empty(gte_graphics->textures);
		texture_t *t = NULL;

		if(index >= 0)
		{
			t = vector_get(gte_graphics->textures, index);
			glGenTextures(1, &t->id);

			glBindTexture(GL_TEXTURE_2D, t->id);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if(channels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			else if(channels == 4)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);

			print_opengl_errors("at %s:%d->%s()\n", __FILE__, __LINE__, __func__);
		}
		return t;
}
