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

#include "model.h"
#include "cglm/types-struct.h"

typedef struct {
	vec3s position;
	vec3s size;
	vec3s velocity;
	vec3s rotation;
	_Bool destroyed;

	model_t *model;
} game_object_t;

game_object_t *game_object_create(vec3s pos, _Bool destroyed);
void game_object_destroy(game_object_t *);

#endif
