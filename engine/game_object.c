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
#include "cglm/cam.h"
#include "cglm/io.h"
#include "cglm/mat4.h"
#include "cglm/vec3.h"
#include <GTEngine/game_object.h>
#include <GTEngine/engine.h>
#include <glad/glad.h>

void game_object_update(game_object_t *g, vec3 target);

game_object_t *game_object_create(model_t *model, vec3 position)
{
	game_object_t *g = malloc(sizeof(game_object_t));
	if(g)
	{
		g->children = vector_create(0, sizeof(game_object_t), 0);
		g->enabled = 1;
		g->parent = NULL;
		g->model = model;
		glm_vec3_zero(g->velocity);
		glm_vec3_copy(position, g->position);
		glm_mat4_identity(g->model_matrix);
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
	glm_vec3_sub(target, g->position, g->direction);
	glm_normalize(g->direction);

	if(fabs(g->direction[0]) < 0.00001 && fabs(g->direction[2]) < 0.00001)
	{
		if(g->direction[1] > 0)
		{
			g->up[0] = 0;
			g->up[1] = 0;
			g->up[2] = -1;
		} else {
			g->up[0] = 0;
			g->up[1] = 0;
			g->up[2] = 1;

		}
	} else {
		g->up[0] = 0;
		g->up[1] = 1;
		g->up[2] = 0;

	}
	glm_normalize(g->up);

	game_object_update(g, target);

}

void game_object_update(game_object_t *g, vec3 target)
{
	glm_cross(g->up, g->direction, g->right);
	glm_normalize(g->right);

	glm_cross(g->direction, g->right, g->up);
	glm_normalize(g->up);

	mat4 m;
	glm_mat4_identity(m);

	glm_vec3_copy(g->right, m[0]);
	glm_vec3_copy(g->up, m[1]);
	glm_vec3_copy(g->direction, m[2]);
	glm_vec3_copy(g->position, m[3]);

	glm_mat4_copy(m, g->model_matrix);
}
