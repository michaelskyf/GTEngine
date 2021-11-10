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
#ifndef GTE_SHADER_H
#define GTE_SHADER_H

#include <cglm/types.h>

typedef struct {
	unsigned int id;

	unsigned int vPos, nPos, tPos; // Shader vertex attribute positions
	unsigned int umPos, uvpPos;// Transformation matrix positions
} shader_t;

shader_t *shader_create(const char *v_path, const char *f_path);
void shader_destroy(shader_t *);

#endif
