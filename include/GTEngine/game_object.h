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
#ifndef GTE_GAME_OBJECT_H
#define GTE_GAME_OBJECT_H

#include <cglm/types.h>
#include "shader.h"
#include "model.h"

typedef struct game_object_t {
	vec3 velocity;

	struct game_object_t *parent;
	struct game_object_t *children;
	size_t childNum;

	mat4 model_matrix;
	model_t *model;
	_Bool enabled;
} game_object_t;

game_object_t *game_object_create(model_t *model, vec3 position);
void game_object_destroy(game_object_t *);

void game_object_draw(game_object_t *, shader_t *);

#endif
