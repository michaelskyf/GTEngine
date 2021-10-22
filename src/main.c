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
	shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs", NULL);

	if(!shader)
	{
		LOGE("Failed to create shader program");
		return -1;
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);


	return 0;
}

void program_update()
{
	glUseProgram(shader->id);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
