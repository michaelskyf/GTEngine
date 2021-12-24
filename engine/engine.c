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

/* External headers */
#include "GTEngine/model.h"
#include "GTEngine/texture.h"
#include "cglm/util.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For chdir
#include <time.h> // For benchmarking

/* Internal headers */
#include <GTEngine/vector.h>
#include <GTEngine/lib.h>
#include <GTEngine/output.h>
#include <GTEngine/camera.h>
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
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

/* Engine variables */
struct gte_time _gte_time;
struct gte_objects _gte_objects;
struct gte_window _gte_window;
struct gte_graphics _gte_graphics;

/* Global variables */
const struct gte_time * const gte_time = &_gte_time;
const struct gte_window * const gte_window = &_gte_window;
struct gte_objects * const gte_objects = &_gte_objects;
struct gte_graphics * const gte_graphics = &_gte_graphics;

/* Local variables */
static GLFWwindow *window;
static shader_t *shader;
static camera_t *camera;

int main(int argc, char **argv)
{
	// For measuring init time
	double startTime = (double)clock()/CLOCKS_PER_SEC;
	/*
	  * Before we do any inits, we need to change working directory
	  * to the directory in which the 'data' resides (same as executable)
	*/
	chdir(dirname(*argv));
	// Restore argv[0]
	argv[0][strlen(*argv)] = '/';

	int (* setup_func[])(void) = {
		print_setup,
		engine_setup,
		opengl_setup,
		game_setup,
	};

	// If engine is set-up, call engine_exit on error
	_Bool engine_done = 0;
	// The same goes for opengl
	_Bool opengl_done = 0;

	// Run setup functions and exit on error
	for(unsigned int i = 0; i < sizeof(setup_func)/sizeof(*setup_func); i++)
	{
		if(setup_func[i]())
		{
			print_error("Error while running setup function %d. Exiting.\n", i);

			if(engine_done)
				engine_exit();
			if(opengl_done)
				opengl_exit();
			return -1;
		}
		if(setup_func[i] == engine_setup)
			engine_done = 1;

		if(setup_func[i] == opengl_setup)
			opengl_done = 1;
	}


	double endTime = (double)clock()/CLOCKS_PER_SEC;

	print_info("Init took %f seconds\n", endTime - startTime);

	// Disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// If everything is properly set-up, run the main loop
	loop();

	// Clean-up then exit
	game_exit();
	opengl_exit();
	engine_exit();

	return 0;
}

static void loop(void)
{
	float last_time = gte_time->get_time();
	float current_time;

	while(!glfwWindowShouldClose(window))
	{
		// Calculate delta-time
		current_time = gte_time->get_time();
		_gte_time.deltaTime = current_time - last_time;
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
	const float radius = 15.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	float camY = sin(glfwGetTime()) * radius;

	glm_vec3_copy((vec3){camX, camY, camZ}, camera->position);

	glUseProgram(shader->id);

	camera_lookat(camera, (vec3){0,0,0});

	camera_bind(camera, shader);

	for(size_t i = 0; i < _gte_objects.objects->size; i++)
	{
		game_object_lookat(vector_get(_gte_objects.objects, i), (vec3){1,0,0});
		game_object_draw(vector_get(_gte_objects.objects, i), shader);
	}
}

static int engine_setup(void)
{
	/* Init _gte_* structs */
	_gte_time.get_time = glfwGetTime;
	_gte_time.deltaTime = 0;

	_gte_window.width = 800;
	_gte_window.height = 600;
	_gte_window.aspect_ratio = (double)_gte_window.width / _gte_window.height;
	glm_perspective(glm_rad(45.0f), _gte_window.aspect_ratio, 0.1f, 100.0f, _gte_window.projection);

	_gte_objects.objects = vector_create(0, sizeof(game_object_t), 2);

	_gte_graphics.textures = vector_create(0, sizeof(texture_t), 0);
	_gte_graphics.models = vector_create(0, sizeof(model_t), 0);

	return 0;
}

static int opengl_setup(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(_gte_window.width, _gte_window.height, "GLTest", NULL, NULL);

	if (!window)
	{
		print_error("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		print_error("Failed to initialize GLAD\n");
		glfwTerminate();
		return -1;
	}

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	glEnable(GL_DEPTH_TEST);

	/* Bind callback functions */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	// Create shader and camera
	shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs");
	camera = camera_create((vec3){0,0,0});

	print_opengl_errors("at %s:%d->%s()\n", __FILE__, __LINE__, __func__);

	return 0;
}

static void engine_exit(void)
{
	// Destroy all game objects
	for(size_t i = 0; i < _gte_objects.objects->size; i++)
		game_object_destroy(vector_get(_gte_objects.objects, i));
	vector_destroy(_gte_objects.objects);
}

static void opengl_exit(void)
{
	glfwTerminate();
}

static void engine_update(void)
{
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);

	// Update window struct
	_gte_window.width = width;
	_gte_window.height = height;
	_gte_window.aspect_ratio = (float)width/height;

	// Update projection matrix
	glm_perspective_resize(_gte_window.aspect_ratio, _gte_window.projection);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX, lastY;
	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera_process_mouse(camera, xoffset, yoffset);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
/* Used for user interface */
{
	static char lastKey;
	static float lastTime;

	if(gte_time->get_time() - lastTime > 0.5f)
		lastKey = -1;

	if(key == lastKey)
		return;

	switch(key)
	{
		case 'Q':
			glfwSetWindowShouldClose(window, 1);
		break;

		case 'C':
			if(gte_window->cursor_enabled)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				_gte_window.cursor_enabled = 0;
			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				_gte_window.cursor_enabled = 1;
			}
		break;
	}
	lastKey = key;
	lastTime = gte_time->get_time();
}
