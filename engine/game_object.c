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
#include "cglm/mat4.h"
#include <GTEngine/vector.h>
#include <GTEngine/game_object.h>
#include <GTEngine/output.h>
#include <cglm/cglm.h>
#include <glad/glad.h>

#include <stdlib.h>

game_object_t *game_object_create(model_t *model, vec3 pos, _Bool destroyed)
{
	game_object_t *game_object = malloc(sizeof(game_object_t));
	if(game_object)
	{
		game_object->model = model;
		game_object->model_matrix = vector_get_element(model->model_matrices, vector_create_element(model->model_matrices));
		glm_mat4_identity(*game_object->model_matrix);
	}
	LOGD("TODO");
	return game_object;
}

void game_object_destroy(game_object_t *game_object)
{
	model_destroy(game_object->model);
}
