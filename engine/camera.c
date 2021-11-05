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
#include <GTEngine/camera.h>
#include <GTEngine/output.h>
#include <cglm/cglm.h>

#include <stdlib.h>

camera_t *camera_create(vec3 position)
{
	LOGD("TODO");
	camera_t *camera = malloc(sizeof(camera_t));
	if(camera)
	{
		glm_vec3_copy(position, camera->position);
		glm_mat4_identity(camera->view);
	}
	return camera;
}

void camera_destroy(camera_t *camera)
{
	free(camera);
}
