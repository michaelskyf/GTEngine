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
#include <libgen.h> // For dirname
#include <unistd.h> // For chdir

/* Internal headers */
#include <GTEngine/config.h> // For title
#include <GTEngine/engine.h>
#include <GTEngine/vector.h>
#include <GTEngine/settings.h>

/* functions */
static int engine_setup(int argc, char **argv);
static int opengl_setup(void);
static void loop(void);

/* callback functions */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

/* Global variables */
struct engine_variables evars;

/* local/static variables */
static struct settings *settings;
static GLFWwindow *window;

int main(int argc, char **argv)
{
	/*
	   Run setup funcions.
	   If any function returns non-0, exit.
	*/
	if (
		engine_setup(argc, argv)
		||
		opengl_setup()
	) {
		LOGE("Error during setup. Exiting.");
		return -1;
	}

	// Everything is set-up,
	// start render loop
	loop();

	glfwTerminate();
	return 0;
}

void loop(void)
{
	while(!glfwWindowShouldClose(window))
	{

		// Clear the back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// check and call events and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

static int engine_setup(int argc, char **argv)
{
	// Before we do any inits,we need to change the directory
	// to executable's path, since data folder should be there.
	// This changes argv[0], so if we need it, we should copy it
	// (we don't need it for now)
	chdir(dirname(*argv));

	// Overwrite default settings with
	// those found in `settings_path`
	evars.settings = settings_default;
	settings = &evars.settings;
	settings_read(settings_path, settings);

	return 0;
}

static int opengl_setup(void)
{
	LOG("Starting openGL setup...");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor);

	window = glfwCreateWindow(settings->width, settings->height, title, NULL, NULL);

	if (!window)
	{
		LOGE("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOGE("Failed to initialize GLAD");
		return -1;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glViewport(0, 0, settings->width, settings->height);

	/* Bind callback functions */
	if(settings->resizable)
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, key_callback);

	LOG("...openGL setup done");
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/*
  * Gets called on key action
*/
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	switch(key)
	{
		case 'Q':
			glfwSetWindowShouldClose(window, 1);
		break;
	}
}
