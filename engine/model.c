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
#include "GTEngine/shader.h"
#include "GTEngine/vector.h"
#include <GTEngine/engine.h>
#include <GTEngine/model.h>
#include <GTEngine/texture.h>
#include <GTEngine/lib.h>
#include <GTEngine/output.h>
#include <assimp/material.h>
#include <assimp/texture.h>
#include <assimp/types.h>
#include <glad/glad.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <string.h>

// Returns mesh_t array of size node->mNumMeshes
static vector_t *process_meshes(const struct aiNode *node, const struct aiScene *scene, const char *directory);
static node_t *process_node(const struct aiNode *node, const struct aiScene *scene, const char *directory);
static vector_t *process_vertices(const struct aiMesh *mMesh);
static vector_t *process_indices(const struct aiMesh *mMesh);
static material_t *process_material(const struct aiMaterial *mMat, const char *directory, const struct aiScene *scene);
static int mesh_setup(mesh_t *m);
static vector_t *material_texture_load(const struct aiMaterial *mMat, enum aiTextureType type, const char *typename, const char *directory, const struct aiScene *scene);
void node_free(node_t *n);

model_t *model_load(const char *path)
{
	model_t *m = malloc(sizeof(model_t));
	if(m)
	{
		// Init variables
		m->node = NULL;
		m->path = path;

		// Check for file extension
		const char *extension = strrchr(basename(path), '.');

		// Validate the extension
		if(!extension || !aiIsExtensionSupported(extension))
		{
			print_error("File extension '%s' not supported by Assmip (\"%s\")\n", extension, path);
			model_destroy(m);
			return NULL;
		}

		// Load the model file
		const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcessPreset_TargetRealtime_Fast);
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene ->mRootNode)
		{
			print_error("Failed to load object \"%s\"\n", path);
			model_destroy(m);
			return NULL;
		}
		m->path = path;
		m->directory = malloc(strlen(path) + 1);
		strcpy((char*)m->directory, path);
		dirname((char*)m->directory);
		m->node = process_node(scene->mRootNode, scene, m->directory);
		m->node->parent = NULL;

		// Destroy scene
		aiReleaseImport(scene);
	}
	return m;
}

void model_destroy(model_t *m)
{
	node_free(m->node);
	free((char*)m->directory);
	free(m);
}

void node_free(node_t *n)
{
	for(size_t i = 0; i < n->children->size; i++)
		node_free(vector_get(n->children, i));
	for(size_t i = 0; i < n->meshes->size; i++)
		mesh_destroy(vector_get(n->meshes, i));


	free(n);
}

void node_draw(node_t *n, shader_t *s)
{
	for(size_t i = 0; i < n->meshes->size; i++)
		mesh_draw(vector_get(n->meshes, i), s);

	if(!n->children)
		return;

	for(size_t i = 0; i < n->children->size; i++)
		node_draw(vector_get(n->children, i), s);
}

void model_draw(model_t *m, shader_t *s)
{
	node_draw(m->node, s);
}

static node_t *process_node(const struct aiNode *node, const struct aiScene *scene, const char *directory)
{
	node_t *n = malloc(sizeof(node_t));
	if(n)
	{
		n->meshes = process_meshes(node, scene, directory);

		if(node->mNumChildren > 0)
			n->children = vector_create(node->mNumChildren, sizeof(node_t), 0);
		else
			n->children = NULL;

		for(size_t i = 0; i < node->mNumChildren; i++)
		{
			node_t *nchild = process_node(node->mChildren[i], scene, directory);
			nchild->parent = n;
			vector_push(n->children, nchild);
			free(nchild);
		}
	}
	return n;
}

static vector_t *process_meshes(const struct aiNode *node, const struct aiScene *scene, const char *directory)
{
	vector_t *meshes = vector_create(node->mNumMeshes, sizeof(mesh_t), 0);

	// Assign vertices and indices
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		mesh_t *mesh = vector_get(meshes, i);
		meshes->size++;
		struct aiMesh *mMesh = scene->mMeshes[node->mMeshes[i]];

		mesh->indices = process_indices(mMesh);
		mesh->vertices = process_vertices(mMesh);
		mesh->material = process_material(scene->mMaterials[mMesh->mMaterialIndex], directory, scene);
		mesh_setup(mesh);
	}
	return meshes;
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

static vector_t *material_texture_load(const struct aiMaterial *mMat, enum aiTextureType type, const char *typename, const char *directory, const struct aiScene *scene)
{
	size_t count = aiGetMaterialTextureCount(mMat, type);
	vector_t *textures = vector_create(count, sizeof(texture_t), 0);

	for(size_t i = 0; i < count; i++)
	{
		struct aiString path;
		aiGetMaterialTexture(mMat, type, i, &path, NULL, NULL, NULL, NULL, NULL, NULL);

		if(!strcmp(path.data, "*0"))
		{
			struct aiTexture *t = scene->mTextures[i];
			if(!t->mHeight)
				t->mHeight = 1;

			texture_t *tex = texture_load_memory((unsigned char*)t->pcData, t->mWidth, t->mHeight);
			vector_push(textures, tex);
			free(tex);

		} else {
			char *file_path = malloc(strlen(directory) + strlen(path.data) + 2);
			strcpy(file_path, directory);
			strcat(file_path, "/");
			strcat(file_path, path.data);

			for(unsigned int y = 0; y < gte_graphics->textures->size; y++)
			{
				texture_t *t = vector_get(gte_graphics->textures, y);
				if(!strcmp(t->type, typename) && !strcmp(t->name, file_path))
				{
					vector_push(textures, t);
					free(file_path);
					goto next;
				}
			}

			texture_t *tex = texture_load(file_path);
			if(tex)
			{
				tex->type = typename;
				tex->name = file_path;
				vector_push(textures, tex);
				vector_push(gte_graphics->textures, tex);
				free(tex);
			} else {
				print_error("Failed to load texture \"%s\"\n", file_path);
			}
		}
next:;
	}
	return textures;
}

static material_t *process_material(const struct aiMaterial *mMat, const char *directory, const struct aiScene *scene)
{
	material_t *material = malloc(sizeof(material_t));
	if(material)
	{
		vector_t *textures = vector_create(0, sizeof(texture_t), 0);

		vector_t *diffuse = material_texture_load(mMat, aiTextureType_DIFFUSE, "texture_diffuse", directory, scene);

		vector_join(textures, diffuse);

		vector_destroy(diffuse);

		material->textures = textures;
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
