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
#ifndef GTE_CAMERA_H
#define GTE_CAMERA_H

#include <cglm/types.h>

typedef struct {
	vec3 position;
	vec3 target;
	vec3 rev_direction;
	vec3 right;
	vec3 up;
	mat4 view;
} camera_t;

camera_t *camera_create(vec3 position);
void camera_destroy(camera_t *);

// Useful for custom camera manipulation functions
void camera_update(camera_t *);

// camera manipulation functions
void camera_move_by(camera_t *, vec3 vector);
void camera_move_to(camera_t *, vec3 position);
void camera_rotate_by(camera_t *, vec3 vector);
void camera_rotate_to(camera_t *, vec3 rotation);

void camera_lookat(camera_t *, vec3 position);

#endif
