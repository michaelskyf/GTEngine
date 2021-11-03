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
#include <cglm/types.h>
#include <GTEngine/output.h>
#include <GTEngine/model.h>
#include <GTEngine/mesh.h>
#include <GTEngine/fileio.h>

#include <stdlib.h>
#include <string.h>

static float vertices_d[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,   // top left
};
static unsigned int indices_d[] = {
    0, 1, 3,   // first triangle
    1, 2, 3,    // second triangle
};


model_t *model_create(const char *tag)
{
	model_t *model = malloc(sizeof(model_t));

	if(!model)
		return NULL;

	model->meshes = vector_create(0, sizeof(mesh_t *));

	if(!tag)
		tag = "NULL";

	size_t tag_len = strlen(tag) + 1;
	model->tag = malloc(tag_len);
	memcpy((char *)model->tag, tag, tag_len);

	return model;
}

model_t *model_load_obj(const char *tag, const char *obj_path)
{
	model_t *model = model_create(tag);
	if(model)
	{
		LOGD("Not implemented yet. Using default values");
		Vector *vertices = vector_create(0, sizeof(vertex_t));
		Vector *indices = vector_create(0, sizeof(unsigned int));

		for(int i = 0; i < sizeof(vertices_d)/sizeof(*vertices_d); i += 3)
		{
			vertex_t vertex;
			vertex.position[0] = vertices_d[i];
			vertex.position[1] = vertices_d[i+1];
			vertex.position[2] = vertices_d[i+2];

			vector_push(vertices, &vertex);
		}

		for(int i = 0; i < sizeof(indices_d)/sizeof(*indices_d); i++)
			vector_push(indices, &indices_d[i]);

		mesh_t *mesh = mesh_create(vertices, indices, NULL);
		vector_push(model->meshes, &mesh);
	}
	return model;
}

void model_destroy(model_t *model)
{
	size_t size = vector_size(model->meshes);
	mesh_t **mesh = vector_start(model->meshes);

	for(size_t i = 0; i < size; i++)
		mesh_destroy(mesh[i]);

	vector_destroy(model->meshes);
	free((char *)model->tag);
	free(model);
}

void model_draw(model_t *model, mat4 *model_matrix)
{
	size_t size = vector_size(model->meshes);
	mesh_t **mesh = vector_start(model->meshes);

	for(size_t i = 0; i < size; i++)
		mesh_draw(mesh[i], model_matrix);
}
