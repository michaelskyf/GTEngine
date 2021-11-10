/*
  * This is the main file of the engine
  * It contains main() function as well as
  * a few setup functions

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

/* External headers */
#include "GTEngine/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For chdir

/* Internal headers */
#include <GTEngine/output.h>
#include <GTEngine/game_object.h>
#include <GTEngine/engine.h>
#include <GTEngine/game.h>

/* Functions */
static int engine_setup(void);
static void engine_update(void);
static int opengl_setup(void);
static void engine_exit(void);
static void opengl_exit(void);
static void loop(void);
static void draw(void);

/* Callback functions */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

/* Global variables */
engine_variables_t *evars;

/* Pointers to evars->* */
static float *delta_time;

/* Local variables */
static GLFWwindow *window;

static const char *dirname(char *path)
{
	char *base = strrchr(path, '/');
	if(base)
		*base='\0';
	return path;
}

int main(int argc, char **argv)
{
	// Before we do any inits, we need to change working directory
	// to the directory in which the main executable resides.
	chdir(dirname(*argv));
	// Restore argv[0]
	argv[0][strlen(*argv)] = '/';

	/*
	   Run setup funcions.
	   If of the functions returns non-0, exit.
	*/
	int (* setup_func[])(void) = {
		print_setup,
		engine_setup,
		opengl_setup,
		game_setup,
	};

	for(unsigned int i = 0; i < sizeof(setup_func)/sizeof(*setup_func); i++)
	{
		if(setup_func[i]())
		{
			LOGE("Error while running setup function %d. Exiting.", i);
			return -1;
		}
	}

	// Everything is set-up,
	// start render loop
	loop();

	// Clean-up then exit
	game_exit();
	opengl_exit();
	engine_exit();

	return 0;
}

static void loop(void)
{
	// Time-keeping variables
	// We init last_time here, because
	// otherwise delta_time would be really big
	// on the first frame
	float last_time = glfwGetTime();
	float current_time;

	while(!glfwWindowShouldClose(window))
	{
		// Calculate delta-time
		current_time = glfwGetTime();
		*delta_time = current_time - last_time;
		last_time = current_time;

		// Clear the back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update the engine
		engine_update();
		// Update the game
		game_update();

		// Draw objects
		draw();

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

static void draw(void)
{
	//LOG("FPS: %d", (int)(1/evars->deltaTime));
	glUseProgram(evars->shader->id);

	// For each loaded game_object (TODO: implement in renderer.c)
	for(size_t i = 0; i < evars->objectCount; i++)
		game_object_draw(evars->objects[i], evars->shader);
}

static int engine_setup(void)
{
	// Create and init evars
	evars = malloc(sizeof(engine_variables_t));

	evars->objects = malloc(0);
	evars->objectCount = 0;

	// Create and init window_s
	struct window_s *win = malloc(sizeof(struct window_s));
		// This values will get overwritten by framebuffer_size_callback
		win->width = 800;
		win->height = 600;
		win->aspect_ratio = (float)win->width/win->height;
		evars->window = win;

	// Init engine.c static variables
	delta_time = (float *)&evars->deltaTime;
	return 0;
}

static int opengl_setup(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(evars->window->width, evars->window->height, "GLTest", NULL, NULL);

	if (!window)
	{
		LOGE("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		LOGE("Failed to initialize GLAD");
		glfwTerminate();
		return -1;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	// Enable depth testing
	//glEnable(GL_DEPTH_TEST);

	/* Bind callback functions */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	// TODO: shaders
	evars->shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs");

	return 0;
}

static void engine_exit(void)
{
	// Free all game objects
	for(size_t i = 0; i < evars->objectCount; i++)
		game_object_destroy(evars->objects[i]);

	// Destroy evars
	free((void*)evars->window);
	free(evars->objects);
	shader_destroy(evars->shader);
	free(evars);
}

static void opengl_exit(void)
{
	glfwTerminate();
}

static void engine_update(void)
{

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);

	// Update window struct
	struct window_s *win = (struct window_s *)evars->window;
	win->width = width;
	win->height = height;
	win->aspect_ratio = (float)width/height;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case 'Q':
			glfwSetWindowShouldClose(window, 1);
		break;
	}
}

size_t game_object_push(game_object_t *g)
{
	size_t index = evars->objectCount;
	evars->objectCount++;
	evars->objects = realloc(evars->objects, evars->objectCount * sizeof(game_object_t));
	evars->objects[index] = g;

	return index;
}

void game_object_pop(size_t index)
{
	memcpy(evars->objects[index], evars->objects[evars->objectCount], sizeof(game_object_t));
	evars->objectCount--;
	evars->objects = realloc(evars->objects, evars->objectCount * sizeof(game_object_t));
}
