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
#ifndef GTE_TEXTURE_H
#define GTE_TEXTURE_H

#include <sys/types.h>

typedef struct texture_t {
	unsigned int id;
	const char *type;
} texture_t;

__attribute__((warn_unused_result))
texture_t *texture_load(const char *path);
__attribute__((warn_unused_result))
texture_t *texture_load_memory(const unsigned char *data, size_t width, size_t height);

#endif
