
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
#ifndef GTE_ENGINE_H
#define GTE_ENGINE_H

#include "shader.h"
#include "game_object.h"

struct window_s {
	unsigned short width, height;
	float aspect_ratio;
	const char *name;
};

typedef struct engine_variables_t {
	// deltaTime tells how much time we spent drawing last frame
	const float deltaTime;
	const struct window_s *window;

	shader_t *shader;

	game_object_t **objects;
	size_t objectCount;
} engine_variables_t;
extern engine_variables_t *evars;

#endif
