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
#include <GTEngine/model.h>
#include <GTEngine/output.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <string.h>

static int model_setup(model_t *model, const char *path);
static void process_node(model_t *model, struct aiNode *node, const struct aiScene *scene);
static mesh_t *process_mesh(struct aiMesh *mesh, const struct aiScene *scene);

model_t *model_load(const char *path)
/* Despite its name model_load only inits model */
/* model_setup actually loads the model */
{
	model_t *m = malloc(sizeof(model_t));
	if(m)
	{
		m->meshes = malloc(0);
		m->mCount = 0;
		model_setup(m, path);
	}
	return m;
}

static const char *basename(const char *path)
/* Helper function for model_load */
{
	char *base = strrchr(path, '/');
	return base ? base+1 : path;
}

static int model_setup(model_t *model, const char *path)
{
	// Check for file extension
	const char *extension = strrchr(basename(path), '.');
	if(!extension)
	{
		LOGE("No extension found in %s", path);
		return -1;
	}

	// Validate the extension
	if(!aiIsExtensionSupported(extension))
	{
		LOGE("File extension '%s' not supported by Assmip", extension);
		return -1;
	}

	// Load the model file
	const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene ->mRootNode)
	{
		LOGE("Assimp failed to load %s", path);
		return -1;
	}

	model->path = path;

	process_node(model, scene->mRootNode, scene);

	return 0;
}

void model_draw(model_t *m)
{
	for(size_t i = 0; i < m->mCount; i++)
		mesh_draw(&m->meshes[i]);
}

static void process_node(model_t *model, struct aiNode *node, const struct aiScene *scene)
{
	// Process all of the node's meshes

	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		struct aiMesh *mMesh = scene->mMeshes[node->mMeshes[i]];
		mesh_t *mesh = process_mesh(mMesh, scene);
		model->meshes = realloc(model->meshes, ++model->mCount * sizeof(mesh_t));
		memcpy(&model->meshes[model->mCount-1], mesh, sizeof(mesh_t));
	}

	// Process the rest recursively
	for(size_t i = 0; i < node->mNumChildren; i++)
		process_node(model, node->mChildren[i], scene);
}

static mesh_t *process_mesh(struct aiMesh *mMesh, const struct aiScene *scene)
{
	// Process vertex positions, normals and texture coordinates
	size_t vCount = mMesh->mNumVertices;
	vertex_t *vertex = malloc(vCount * sizeof(vertex_t));
	for(size_t i = 0; i < mMesh->mNumVertices; i++)
	{
		// positions
		vertex->position[0] = mMesh->mVertices[i].x;
		vertex->position[1] = mMesh->mVertices[i].y;
		vertex->position[2] = mMesh->mVertices[i].z;
		// normals
		vertex->normals[0] = mMesh->mNormals[i].x;
		vertex->normals[1] = mMesh->mNormals[i].y;
		vertex->normals[2] = mMesh->mNormals[i].z;
		// texture coordinates
		if(mMesh->mTextureCoords[0])
		{
			vertex->texCoords[0] = mMesh->mTextureCoords[0][i].x;
			vertex->texCoords[1] = mMesh->mTextureCoords[0][i].y;
		} else {
			vertex->texCoords[0] = 0;
			vertex->texCoords[1] = 0;
		}
		vertex++;
	}
	vertex -= vCount;
	// All vertices are processed
	// Now, it's time for indices

	// We need to malloc sizeof(uint) * number of faces * number of indices on each face
	size_t iCount = 0;
	iCount = mMesh->mFaces[0].mNumIndices * mMesh->mNumFaces;
	unsigned int *indices = malloc(iCount * sizeof(unsigned int));
	for(size_t i = 0; i < mMesh->mNumFaces; i++)
	{
		struct aiFace face = mMesh->mFaces[i];
		for(size_t j = 0; j < face.mNumIndices; j++)
		{
			indices[j] = face.mIndices[j];
		}
		indices += face.mNumIndices;
	}
	indices -= iCount;

	// Create dummy material
	static material_t *material;
	if(!material)
	{
		material = malloc(sizeof(material_t));
		shader_t *shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs");
		material->shader = shader;
		material->tCount = 0;
	}
	mesh_t *mesh = mesh_create(vertex, vCount, indices, iCount, material);
	return mesh;
}
