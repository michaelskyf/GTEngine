/*
  * This file is for testing only, since I'm trying to relearn openGL.
*/

#include "GTEngine/vector.h"
#include <GTEngine/engine.h>
#include <GTEngine/shader.h>
#include <GTEngine/fileio.h>
#include <GTEngine/mesh.h>
#include <GTEngine/model.h>
#include <GTEngine/game_object.h>
#include "init.h"

#include <glad/glad.h>

int program_setup()
{
	int (* setup_func[])(void) = {
		shaders_setup,
		models_setup,
		game_objects_setup,
	};

	for(unsigned int i = 0; i < sizeof(setup_func)/sizeof(*setup_func); i++)
	{
		if(setup_func[i]())
		{
			LOGE("Error while running program setup function %d.", i);
			return -1;
		}
	}


	return 0;
}

void program_exit(void)
{
}

void program_update()
{
}
