#include <GTEngine/output.h>
#include <GTEngine/model.h>
#include <GTEngine/mesh.h>

#include <stdlib.h>

model_t *model_create(void)
{
	model_t *model = malloc(sizeof(model_t));
	if(model)
	{
		model->meshes = vector_create(0, sizeof(mesh_t *));
		model->model_matrices = vector_create(0, sizeof(mat4));
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
	vector_destroy(model->model_matrices);
	free(model);
}

void model_draw(model_t *model)
{
	size_t size = vector_size(model->meshes);
	mesh_t **mesh = vector_start(model->meshes);

	for(size_t i = 0; i < size; i++)
		mesh_draw(mesh[i]);
}
