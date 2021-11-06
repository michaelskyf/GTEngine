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
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stdlib.h>

static int model_setup(model_t *model, const char *path);
static void process_node(struct aiNode *node, const struct aiScene *scene);

model_t *model_load(const char *path)
/* Despite its name model_load only inits model */
/* model_setup actually loads the model */
{
	model_t *m = malloc(sizeof(model_t));
	if(m)
	{
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

	process_node(scene->mRootNode, scene);

	return 0;
}

static void process_node(struct aiNode *node, const struct aiScene *scene)
{

}
