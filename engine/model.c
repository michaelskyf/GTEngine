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
#include "GTEngine/mesh.h"
#include "GTEngine/vector.h"
#include <GTEngine/model.h>
#include <GTEngine/texture.h>
#include <GTEngine/lib.h>
#include <GTEngine/output.h>
#include <assimp/material.h>
#include <glad/glad.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <string.h>

// Returns mesh_t array of size node->mNumMeshes
static mesh_t *process_meshes(const struct aiNode *node, const struct aiScene *scene);
static mesh_t *process_node(const struct aiNode *node, const struct aiScene *scene);
static vector_t *process_vertices(const struct aiMesh *mMesh);
static vector_t *process_indices(const struct aiMesh *mMesh);
static material_t *process_material(const struct aiMesh *mMesh);
static int mesh_setup(mesh_t *m);

model_t *model_load(const char *path)
{
	model_t *m = malloc(sizeof(model_t));
	if(m)
	{
		// Init variables
		m->mesh = NULL;
		m->path = path;

		// Check for file extension
		const char *extension = strrchr(basename(path), '.');
		if(!extension)
		{
			LOGE("No extension found in %s", path);
			model_destroy(m);
			return NULL;
		}

		// Validate the extension
		if(!aiIsExtensionSupported(extension))
		{
			LOGE("File extension '%s' not supported by Assmip", extension);
			model_destroy(m);
			return NULL;
		}

		// Load the model file
		const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs
				| aiProcessPreset_TargetRealtime_Fast);
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene ->mRootNode)
		{
			LOGE("Assimp failed to load %s", path);
			model_destroy(m);
			return NULL;
		}
		m->mesh = process_node(scene->mRootNode, scene);

		// Destroy scene
		aiReleaseImport(scene);
	}
	return m;
}

void model_destroy(model_t *m)
{
}

void model_draw(model_t *m, shader_t *s)
{
	for(size_t i = 0; i < m->mesh->children->size; i++)
		mesh_draw(vector_get(m->mesh->children, i), s);
}

static mesh_t *process_node(const struct aiNode *node, const struct aiScene *scene)
{
	/*
	   * Each node should correspond to a mesh
	*/

	// Init mesh
	mesh_t *mesh = process_meshes(node, scene);


	// Process children
	vector_t *children = vector_create(node->mNumChildren, sizeof(mesh_t), 0);

	for(size_t i = 0; i < children->capacity; i++)
	{
		const struct aiNode *child = node->mChildren[i];

		mesh_t *m = process_node(child, scene);
		vector_push(children, m);
		free(m);
	}
	mesh->children = children;

	return mesh;
}

static mesh_t *process_meshes(const struct aiNode *node, const struct aiScene *scene)
/* Returns one mesh made of all of the node's meshes */
{
	vector_t *vertices = NULL;
	vector_t *indices = NULL;

	// Assign vertices and indices
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		struct aiMesh *mMesh = scene->mMeshes[node->mMeshes[i]];
		indices = process_indices(mMesh);
		vertices = process_vertices(mMesh);
	}

	// Finally, create the mesh
	mesh_t *m = malloc(sizeof(mesh_t));
	if(m)
	{
		m->vertices = vertices;
		m->indices = indices;
		m->material = NULL;

		if(m->vertices && m->indices)
			mesh_setup(m);
	}
	return m;
}

static vector_t *process_vertices(const struct aiMesh *mMesh)
{
	vector_t *vertices = vector_create(mMesh->mNumVertices, sizeof(vertex_t), 0);

	// For each vertex:
	for(size_t i = 0; i < vertices->capacity; i++)
	{
		vertex_t vertex;
		// Copy vertex positions
		glm_vec3_copy(&mMesh->mVertices[i].x, vertex.position);
		// Copy normals
		glm_vec3_copy(&mMesh->mNormals[i].x, vertex.normals);
		// Copy texture coordinates if they exist
		if(mMesh->mTextureCoords[0])
			glm_vec2_copy(&mMesh->mTextureCoords[0][i].x, vertex.texCoords);
		else
			glm_vec2_zero(vertex.texCoords);

		vector_push(vertices, &vertex);
	}
	return vertices;
}

static vector_t *process_indices(const struct aiMesh *mMesh)
{
	// Create an array of unsigned ints
	// Since we used 'aiProcess_Triangulate' flag, size of indices is equal to faces * 3
	vector_t *indices = vector_create(mMesh->mNumFaces * 3, sizeof(unsigned short), 0);

	// For each face:
	for(size_t f = 0; f < mMesh->mNumFaces; f++)
	{
		struct aiFace *face = &mMesh->mFaces[f];
		// For each index:
		for(size_t i = 0; i < face->mNumIndices; i++)
			vector_push(indices, &face->mIndices[i]);
	}
	return indices;
}

static material_t *process_material(const struct aiMesh *mMesh)
{
	// Create a dummy material
	static material_t *material;
	if(!material)
	{
		LOGW("Creating a dummy material");
		material = malloc(sizeof(material_t));
	}
	return material;
}

static int mesh_setup(mesh_t *m)
{
	// Generate buffers
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ebo);

	// Setup buffers data
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, m->vertices->size * m->vertices->item_size, m->vertices->data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->indices->size * m->indices->item_size, m->indices->data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return 0;
}
