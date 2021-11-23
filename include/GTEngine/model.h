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
#ifndef GTE_MODEL_H
#define GTE_MODEL_H

#include "mesh.h"
#include "vector.h"

typedef struct node {
	vector_t *meshes;

	const struct node *parent;
	vector_t *children;
} node_t;

typedef struct model_t {
	node_t *node;
	const char *path;
	const char *directory;
} model_t;

__attribute_warn_unused_result__
model_t *model_load(const char *path);
void model_destroy(model_t *);
void model_draw(model_t *, shader_t *);

#endif
