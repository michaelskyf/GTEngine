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

#include <GTEngine/shader.h>
#include <GTEngine/fileio.h>
#include <GTEngine/macros.h>

#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static int shader_piece_create(unsigned int *shader, const char *path, int type);

void shader_destroy(shader_t *shader)
{
	glDeleteShader(shader->id);
	free(shader);
}

shader_t *shader_create(const char *v_path, const char *f_path, const char *g_path)
{
	shader_t *shader = malloc(sizeof(shader_t));
	unsigned int vertex, fragment, geometry;

	if(shader_piece_create(&vertex, v_path, GL_VERTEX_SHADER))
	{
		LOGE("Failed to create vertex shader");
		free(shader);
		return NULL;
	}

	if(shader_piece_create(&fragment, f_path, GL_FRAGMENT_SHADER))
	{
		LOGE("Failed to create fragment shader");
		glDeleteShader(vertex);
		free(shader);
		return NULL;
	}

	if(g_path && shader_piece_create(&geometry, g_path, GL_GEOMETRY_SHADER))
	{
		LOGE("Failed to create geometry shader");
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		free(shader);
		return NULL;
	}

	shader->id = glCreateProgram();

	if(!shader->id)
	{
		LOGE("Failed to create shader program");
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if(g_path)
			glDeleteShader(geometry);
		free(shader);
		return NULL;
	}

	glAttachShader(shader->id, vertex);
	glAttachShader(shader->id, fragment);
	if(g_path)
		glAttachShader(shader->id, geometry);

	glLinkProgram(shader->id);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if(g_path)
		glDeleteShader(geometry);

	int success;
	glGetProgramiv(shader->id, GL_LINK_STATUS, &success);

	if(!success)
	{
		char infolog[1024];
		glGetShaderInfoLog(shader->id, 1024, NULL, infolog);
		LOGE("Shader program compilation failed:\n%s", infolog);
		glDeleteProgram(shader->id);
		free(shader);
		return NULL;
	}

	return shader;
}

static int shader_piece_create(unsigned int *shader, const char *path, int type)
{
	// if path is NULL
	if(!path)
		return -1;

	const char *shader_source = file_read(path);

	if (!shader_source)
		return -1;

	*shader = glCreateShader(type);

	glShaderSource(*shader, 1, &shader_source, NULL);
	glCompileShader(*shader);

	free((char*)shader_source);

	int success;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		char infolog[1024];
		glGetShaderInfoLog(*shader, 1024, NULL, infolog);
		LOGE("Shader compilation failed:\n%s:\n%s", path, infolog);
		return -1;
	}

	return 0;
}
