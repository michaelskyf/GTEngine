/*
  * This file is for testing only, since I'm trying to relearn openGL.
*/

#include <GTEngine/engine.h>
#include <GTEngine/shader.h>
#include <GTEngine/fileio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// for triangle
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

static shader_t *shader;
unsigned int vao;

int program_setup()
{
	unsigned int vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
   	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs", NULL);

	if(!shader)
	{
		LOGE("Failed to create shader program");
		return -1;
	}


	return 0;
}

void program_update()
{
	glUseProgram(shader->id);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
