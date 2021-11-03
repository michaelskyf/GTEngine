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
#include <GTEngine/engine.h>
#include <GTEngine/shader.h>
#include <GTEngine/vector.h>
#include <GTEngine/model.h>
#include <GTEngine/mesh.h>
#include <GTEngine/game_object.h>
#include "init.h"

int shaders_setup(void)
{
	shader_t *shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs", "default");
	vector_push(evars.shaders, &shader);
	return 0;
}

int models_setup(void)
{
	model_t *model = model_load_obj("square", "some/path.obj");
	vector_push(evars.models, &model);

	return 0;
}

int game_objects_setup(void)
{
	model_t **model = vector_start(evars.models);
	game_object_t *go = game_object_create(*model, (vec3){0,0,0}, 1);
	vector_push(evars.game_objects, &go);

	return 0;
}
