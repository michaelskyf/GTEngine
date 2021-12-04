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
#include "GTEngine/vector.h"
#include "cglm/affine-mat.h"
#include "cglm/affine.h"
#include "cglm/cam.h"
#include "cglm/io.h"
#include "cglm/mat4.h"
#include "cglm/vec3.h"
#include <GTEngine/game_object.h>
#include <GTEngine/engine.h>
#include <glad/glad.h>
#include <math.h>

game_object_t *game_object_create(model_t *model, vec3 position, vec3 scale)
{
	game_object_t *g = malloc(sizeof(game_object_t));
	if(g)
	{
		g->children = vector_create(0, sizeof(game_object_t), 0);
		g->enabled = 1;
		g->parent = NULL;
		g->model = model;
		glm_mat4_identity(g->model_matrix);
		glm_translate(g->model_matrix, position);
		glm_scale(g->model_matrix, scale);
	}
	return g;
}

void game_object_destroy(game_object_t *g)
{
	free(g);
}

void game_object_draw(game_object_t *g, shader_t *s)
{
	glUniformMatrix4fv(s->umPos, 1, GL_FALSE, *g->model_matrix);
	model_draw(g->model, s);
}

void game_object_lookat(game_object_t *g, vec3 target)
{
	vec3 scale;
	vec4 position;
	mat4 rotation;

	glm_decompose(g->model_matrix, position, rotation, scale);

	glm_mat4_identity(g->model_matrix);
	glm_lookat(position, target, (vec3){0,1,0}, g->model_matrix);
	glm_mat4_inv(g->model_matrix, g->model_matrix);
	glm_scale(g->model_matrix, scale);
}

void game_object_rotate(game_object_t *g, float angle, vec3 axis)
{
	glm_rotate(g->model_matrix, angle, axis);
}

void game_object_rotate_make(game_object_t *g, float angle, vec3 axis)
{
	glm_rotate_make(g->model_matrix, angle, axis);
}
