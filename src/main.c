/*
  * This file is for testing only, since I'm trying to relearn openGL.
*/

#include <GTEngine/engine.h>
#include <GTEngine/shader.h>
#include <GTEngine/fileio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

unsigned int ebo;
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

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	return 0;
}

void program_update()
{
	glUseProgram(shader->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
