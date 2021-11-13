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
#include "GTEngine/mesh.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/vec3.h"
#include <GTEngine/game_object.h>
#include <GTEngine/engine.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

game_object_t *game_object_create(model_t *model, vec3 position)
{
	game_object_t *g = malloc(sizeof(game_object_t));
	if(g)
	{
		g->childNum = 0;
		g->enabled = 1;
		g->parent = NULL;
		g->model = model;
		glm_vec3_zero(g->velocity);
		glm_mat4_identity(g->model_matrix);
		glm_translate_to(g->model_matrix, position, g->model_matrix);
	}
	return g;
}

void game_object_destroy(game_object_t *g)
{
	free(g);
}

void game_object_draw(game_object_t *g, shader_t *s)
{
	glad_glUniformMatrix4fv(s->umPos, 1, GL_FALSE, *g->model_matrix);
	model_draw(g->model, s);
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
