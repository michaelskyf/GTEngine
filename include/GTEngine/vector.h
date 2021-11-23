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
#ifndef GTE_VECTOR_H
#define GTE_VECTOR_H
#include <sys/cdefs.h>
#include <sys/types.h>

typedef struct vector_t
{
	size_t size;
	size_t item_size;
	float growth_rate;
	size_t capacity;
	void *data;
} vector_t;

__attribute_warn_unused_result__
vector_t *vector_create(size_t capacity, size_t item_size, size_t growth_rate);
void vector_destroy(vector_t *);

int vector_push(vector_t *, const void *);
void vector_pop(vector_t *, void *);

__attribute_warn_unused_result__
void *vector_get(vector_t *, size_t index);
int vector_join(vector_t *dest, vector_t *src);

#endif
