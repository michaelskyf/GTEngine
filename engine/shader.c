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
		free(shader);
		return NULL;
	}

	if(shader_piece_create(&fragment, f_path, GL_FRAGMENT_SHADER))
	{
		glDeleteShader(vertex);
		free(shader);
		return NULL;
	}

	if(shader_piece_create(&geometry, g_path, GL_GEOMETRY_SHADER))
	{
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		free(shader);
		return NULL;
	}

	shader->id = glCreateProgram();

	glAttachShader(shader->id, vertex);
	glAttachShader(shader->id, fragment);
	if(g_path)
		glAttachShader(shader->id, fragment);

	glLinkProgram(shader->id);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if(g_path)
		glDeleteShader(geometry);

	int success;
	glGetShaderiv(shader->id, GL_COMPILE_STATUS, &success);

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
