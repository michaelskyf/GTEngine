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
#include "cglm/affine.h"
#include "cglm/cam.h"
#include "cglm/cglm.h"
#include "cglm/util.h"
#include "cglm/vec3.h"
#include <GTEngine/camera.h>
#include <GTEngine/engine.h>
#include <glad/glad.h>
#include <stdlib.h>

camera_t *camera_create(void)
{
	camera_t *c = malloc(sizeof(camera_t));
	if(c)
	{
		c->pitch = 0.0f;
		c->yaw = 0.0f;
		camera_update(c);
	}
	return c;
}

void camera_destroy(camera_t *c)
{
	free(c);
}

void camera_update(camera_t *c)
{
	printf("p: %f y: %f\n", c->pitch, c->yaw);
	double yaw = glm_rad(c->yaw);
	double pitch = glm_rad(c->pitch);

	// calculate front vector
	c->front[0] = cos(yaw) * cos(pitch);
	c->front[1] = sin(pitch);
	c->front[2] = sin(yaw) * cos(pitch);
	glm_normalize(c->front);

	// calculate right vector
	glm_cross(c->front, (vec3){0,1,0}, c->right);
	glm_normalize(c->right);

	// calculate up vector
	glm_cross(c->right, c->front, c->up);
	glm_normalize(c->up);

	// Update the view matrix
	vec3 pf;
	glm_vec3_add(c->position, c->front, pf);
	glm_lookat(c->position, pf, c->up, c->view_matrix);
}

void camera_process_mouse(camera_t *c, double xoffset, double yoffset)
{
	float sens = 0.1f;
	c->yaw += xoffset * sens;
	c->pitch += yoffset * sens;

	if(c->pitch > 89.0f)
		c->pitch = 89.0f;
	if(c->pitch < -89.0f)
		c->pitch = -89.0f;

	camera_update(c);
}

void camera_bind(camera_t *c, shader_t *s)
{
	mat4 vp_matrix;
	glm_mat4_mul(evars->projection_matrix, c->view_matrix, vp_matrix);
	glUniformMatrix4fv(s->uvpPos, 1, GL_FALSE, *vp_matrix);
}
