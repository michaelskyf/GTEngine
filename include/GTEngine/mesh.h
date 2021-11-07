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
#ifndef GTE_MESH_H
#define GTE_MESH_H
#include <cglm/cglm.h>

#include "shader.h"

typedef struct vertex_t {
	vec3 position;
	vec3 normals;
	vec2 texCoords;
} vertex_t;

typedef struct texture_t {
	unsigned int id;
	const char *type;
} texture_t;

typedef struct material_t {
	shader_t *shader;
	texture_t *textures;
	size_t tCount;
} material_t;

typedef struct mesh_t {
	vertex_t *vertices;
	size_t verticesCount;
	unsigned int *indices;
	size_t indicesCount;
	material_t *material;

	struct mesh_t *parent;
	struct mesh_t *children;
	size_t childrenCount;

	unsigned int vbo, ebo;
} mesh_t;

mesh_t *mesh_create(vertex_t *, size_t vCount, unsigned int *, size_t iCount,
			material_t *);
void mesh_destroy(mesh_t *);
void mesh_draw(mesh_t *);

#endif
