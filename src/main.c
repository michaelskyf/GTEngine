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

int program_setup()
{
	// Create Vertex Buffer Object to store vertices inside GPU
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	shader_create("data/shaders/test.vs", "data/shaders/test.fs", NULL);

	return 0;
}

void program_update()
{

}
