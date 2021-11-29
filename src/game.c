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
#include <GTEngine/vector.h>
#include <GTEngine/shader.h>
#include <GTEngine/game_object.h>
#include <GTEngine/output.h>
#include <GTEngine/engine.h>

model_t *model;
shader_t *shader;
game_object_t *GO;

int game_setup(void)
{
	struct gte_objects *objects = (struct gte_objects*)gte_objects;
	model = model_load("data/objects/backpack.obj");
	GO = game_object_create(model, (vec3){0,0,0});
	vector_push(objects->objects, GO);

	return 0;
}

void game_exit(void)
{

	LOG("bye");
}

void game_update(void)
{
}
