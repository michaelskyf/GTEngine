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
#include <GTEngine/game_object.h>
#include <GTEngine/output.h>
#include <cglm/cglm.h>

#include <stdlib.h>

game_object_t *game_object_create(model_t *model, vec3 pos, _Bool enabled)
{
	game_object_t *g = malloc(sizeof(game_object_t));
	if(g)
	{
		glm_vec3_copy(pos, g->position);
		glm_vec3_one(g->size);
		glm_vec3_zero(g->rotation);

		g->enabled = enabled;

		glm_mat4_identity(g->model_matrix);
		g->model = model;
	}
	return g;
}

void game_object_destroy(game_object_t *g)
{
	// We shouldn't destroy model, since it can be tied
	// to many game_objects
	free(g);
}

void game_object_draw(game_object_t *g)
{
	model_draw(g->model, &g->model_matrix);
}
