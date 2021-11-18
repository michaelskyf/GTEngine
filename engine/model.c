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
#include <GTEngine/model.h>
#include <GTEngine/output.h>
#include <glad/glad.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>
#include <string.h>

static const char *basename(const char *path);
// Returns mesh_t array of size node->mNumMeshes
static mesh_t *process_meshes(const struct aiNode *node, const struct aiScene *scene);
static void process_root_node(model_t *model, const struct aiScene *scene);
static vertex_t *process_vertices(const struct aiMesh *mMesh);
static unsigned short *process_indices(const struct aiMesh *mMesh);
static material_t *process_material(const struct aiMesh *mMesh);
static int mesh_setup(mesh_t *m);

model_t *model_load(const char *path)
{
	model_t *m = malloc(sizeof(model_t));
	if(m)
	{
		// Init variables
		m->meshCount = 0;
		m->meshes = NULL;
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
		// Init meshes and meshCount
		process_root_node(m, scene);

		// Destroy scene
		aiReleaseImport(scene);
	}
	return m;
}

void model_destroy(model_t *m)
{
	if(m->meshes)
		free(m->meshes);
	// TODO remove children
	free(m);
}

void model_draw(model_t *m, shader_t *s)
{
	for(size_t i = 0; i < m->meshCount; i++)
		mesh_draw(&m->meshes[i], s);
}

static const char *basename(const char *path)
/* Helper function for model_load */
{
	char *base = strrchr(path, '/');
	return base ? base+1 : path;
}

static void process_root_node(model_t *model, const struct aiScene *scene)
{
	const struct aiNode *root = scene->mRootNode;
	size_t array_size = 0;
	// For each child of the root node:
	for(size_t i = 0; i < root->mNumChildren; i++)
	{
		// Count number of meshes
		const struct aiNode *child = root->mChildren[i];
		array_size += child->mNumMeshes;
	}

	mesh_t *mesh_array = malloc(array_size * sizeof(mesh_t));
	size_t array_index = 0;
	mesh_t *new_array;
	// For each child of the root node:
	for(size_t i = 0; i < root->mNumChildren; i++)
	{
		// Get meshes
		const struct aiNode *child = root->mChildren[i];
		new_array = process_meshes(child, scene);

		// Copy meshes to mesh_array
		memcpy(&mesh_array[array_index], new_array, child->mNumMeshes * sizeof(mesh_t));
		array_index++;
		free(new_array);
	}
	// Set meshes and meshCount
	model->meshes = mesh_array;
	model->meshCount = array_size;
}

static mesh_t *process_meshes(const struct aiNode *node, const struct aiScene *scene)
{
	// Create an array of meshes
	size_t array_size = node->mNumMeshes;
	mesh_t *mesh_array = malloc(array_size * sizeof(mesh_t));

	// For each mesh:
	for(size_t i = 0; i < array_size; i++)
	{
		mesh_t *mesh = &mesh_array[i];
		const struct aiMesh *mMesh = scene->mMeshes[node->mMeshes[i]];
		// Process vertices
		mesh->vertices = process_vertices(mMesh);
		mesh->verticesCount = mMesh->mNumVertices;
		// Process indices
		// Since we used 'aiProcess_Triangulate' flag, size of indices is equal to faces * 3
		mesh->indices = process_indices(mMesh);
		mesh->indicesCount = mMesh->mNumFaces * 3;
		// Process material
		mesh->material = process_material(mMesh);
		//Finally, setup the mesh
		mesh_setup(mesh);
	}
	return mesh_array;
}

static vertex_t *process_vertices(const struct aiMesh *mMesh)
{
	// Create an array of vertices
	size_t array_size = mMesh->mNumVertices;
	vertex_t *vertex_array = malloc(array_size * sizeof(vertex_t));

	// For each vertex:
	for(size_t i = 0; i < array_size; i++)
	{
		vertex_t *vertex = &vertex_array[i];
		// Copy vertex positions
		glm_vec3_copy(&mMesh->mVertices[i].x, vertex->position);
		// Copy normals
		glm_vec3_copy(&mMesh->mNormals[i].x, vertex->normals);
		// Copy texture coordinates if they exist
		if(mMesh->mTextureCoords[0])
			glm_vec2_copy(&mMesh->mTextureCoords[0][i].x, vertex->texCoords);
		else
			glm_vec2_zero(vertex->texCoords);
	}
	return vertex_array;
}

static unsigned short *process_indices(const struct aiMesh *mMesh)
{
	// Create an array of unsigned ints
	// Since we used 'aiProcess_Triangulate' flag, size of indices is equal to faces * 3
	size_t array_size = mMesh->mNumFaces * 3;
	unsigned short *index_array = malloc(array_size * sizeof(unsigned int));

	// For each face:
	for(size_t f = 0; f < mMesh->mNumFaces; f++)
	{
		struct aiFace *face = &mMesh->mFaces[f];
		// For each index:
		for(size_t i = 0; i < face->mNumIndices; i++)
		{
			index_array[f*3+i] = face->mIndices[i];
		}
	}
	return index_array;
}

static material_t *process_material(const struct aiMesh *mMesh)
{
	// Create a dummy material
	static material_t *material;
	if(!material)
	{
		LOGW("Creating a dummy material");
		material = malloc(sizeof(material_t));
		material->tCount = 0;
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
	glBufferData(GL_ARRAY_BUFFER, m->verticesCount * sizeof(vertex_t), m->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->indicesCount * sizeof(*m->indices), m->indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return 0;
}
