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

#include "vector.h"
#include "shader.h"

typedef struct {
	float position[3];
	float normal[3];
	float texture_coords[3];
} vertex_t;

typedef struct {
	unsigned int id;
} texture_t;

typedef struct {
	Vector *vertices;
	Vector *indices;
	Vector *textures;

	unsigned int ebo, vbo;
} mesh_t;

mesh_t *mesh_create(Vector *vertices, Vector *indices, Vector *textures, shader_t *);
void mesh_destroy(mesh_t *);

#endif
