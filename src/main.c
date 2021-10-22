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

int program_setup()
{
	// Create Vertex Buffer Object to store vertices inside GPU
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
