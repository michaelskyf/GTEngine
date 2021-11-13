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
#include "cglm/affine.h"
#include "cglm/cglm.h"
#include <GTEngine/shader.h>
#include <GTEngine/game_object.h>
#include <GTEngine/output.h>
// For testing only
#include <GTEngine/model.h>
#include <GLFW/glfw3.h>

model_t *model;
shader_t *shader;
game_object_t *GO;

int game_setup(void)
{
	model = model_load("data/backpack.obj");
	GO = game_object_create(model, (vec3){10,0,0});
	game_object_push(GO);
	GO = game_object_create(model, (vec3){-10,0,0});
	game_object_push(GO);
	GO = game_object_create(model, (vec3){0,0,0});
	game_object_push(GO);
	GO = game_object_create(model, (vec3){0,10,10});
	game_object_push(GO);
	GO = game_object_create(model, (vec3){0,-10,-10});
	game_object_push(GO);

	return 0;
}

void game_exit(void)
{

}

void game_update(void)
{
	glm_rotate(GO->model_matrix, 0.05, (vec3){0,1,0});
}
