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
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // For dirname
#include <unistd.h> // For chdir

/* Internal headers */
#include <GTEngine/config.h>
#include <GTEngine/engine.h>
#include <GTEngine/vector.h>
#include <GTEngine/settings.h>
#include <GTEngine/game_object.h>
#include <GTEngine/shader.h>
#include <GTEngine/camera.h>

/* Functions */
static int engine_setup(void);
static int opengl_setup(void);
static void engine_exit(void);
static void opengl_exit(void);
static void loop(void);
static void opengl_draw(void);

/* Callback functions */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

/* Global variables */
struct engine_variables evars;

/* Local variables */
static GLFWwindow *window;

// Contains pointers to shader programs
static Vector *shaders;
// Contains pointers to game_objects
static Vector *game_objects;

int main(int argc, char **argv)
{
	// Before we do any inits, we need to change working directory
	// to the directory in which the main executable resides.
	//
	// This function changes argv[0], but we don't need it for anything
	chdir(dirname(*argv));

	/*
	   Run setup funcions.
	   If of the functions returns non-0, exit.
	*/
	int (* setup_func[])(void) = {
		print_setup,
		engine_setup,
		opengl_setup,
		program_setup,
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
	program_exit();
	opengl_exit();
	engine_exit();

	return 0;
}

static void loop(void)
{
	while(!glfwWindowShouldClose(window))
	{
		// Clear the back buffer
		// temporarly moved
		glClear(GL_COLOR_BUFFER_BIT);

		// Update program
		program_update();

		// Draw objects
		opengl_draw();

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

static void opengl_draw(void)
{

}

static int engine_setup(void)
{
	// Initialize settings
	evars.settings = settings_create();

	// Overwrite default settings with
	// those found in `settings_path`
	settings_read(settings_path, evars.settings);

	// Initialize shaders and game_objects
	game_objects = vector_create(0, sizeof(game_object_t *));
	shaders = vector_create(0, sizeof(shader_t *));
	evars.game_objects = game_objects;
	evars.shaders = shaders;

	return 0;
}

static int opengl_setup(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(evars.settings->width, evars.settings->height, title, NULL, NULL);

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

	glViewport(0, 0, evars.settings->width, evars.settings->height);

	/* Bind callback functions */
	if(evars.settings->resizable)
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);

	return 0;
}

static void engine_exit(void)
{
	// Destroy all shaders
	shader_t *shader = vector_start(shaders);
	for(size_t i = 0; i < vector_size(shaders); i++)
		shader_destroy(&shader[i]);

	// Destroy all game objects
	game_object_t *g_obj = vector_start(game_objects);
	for(size_t i = 0; i < vector_size(game_objects); i++)
		game_object_destroy(&g_obj[i]);

	// Destroy evars
	settings_destroy(evars.settings);
	vector_destroy(evars.game_objects);
	vector_destroy(evars.shaders);
}

static void opengl_exit(void)
{
	glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
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
