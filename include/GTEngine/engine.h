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

#include <cglm/types.h>
#include "vector.h"

extern struct gte_time
{
	double deltaTime;
	double (*get_time)(void); // Time in seconds the from the start of the program
} const * const gte_time;

extern struct gte_window
{
	unsigned short width, height;
	double aspect_ratio;
	_Bool cursor_enabled;
	mat4 projection;
} const * const gte_window;

extern struct gte_objects
{
	vector_t *objects;
	vector_t *cameras;
} * const gte_objects;

extern struct gte_graphics
{
	vector_t *models;
	vector_t *textures;
} const * const gte_graphics;

#endif
