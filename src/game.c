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
#include "GTEngine/shader.h"
#include <GTEngine/output.h>
// For testing only
#include <GTEngine/mesh.h>
#include <GTEngine/vector.h>
#include <GTEngine/model.h>

static float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,   // top left
};
static unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3,    // second triangle
};

static mesh_t *mesh;

int game_setup(void)
{
	Vector *vv = vector_create(0, sizeof(vertex_t));
	Vector *iv = vector_create(0, sizeof(unsigned int));

	for(int i = 0; i < sizeof(vertices)/sizeof(*vertices); i += 3)
	{
		vertex_t vertex;
		vertex.position[0] = vertices[i];
		vertex.position[1] = vertices[i+1];
		vertex.position[2] = vertices[i+2];

		vector_push(vv, &vertex);
	}

	for(int i = 0; i < sizeof(indices)/sizeof(*indices); i++)
		vector_push(iv, &indices[i]);

	material_t *material = malloc(sizeof(material_t));
	shader_t *shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs");

	material->shader = shader;

	mesh = mesh_create(vector_start(vv), vector_size(vv), vector_start(iv), vector_size(iv), material);

	model_load(__FILE__);

	return 0;
}

void game_exit(void)
{

}

void game_update(void)
{
	mesh_draw(mesh);
}
