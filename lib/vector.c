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
#include <GTEngine/vector.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

vector_t *vector_create(size_t capacity, size_t item_size, size_t growth_rate)
{
	vector_t *v = malloc(sizeof(vector_t));
	if(v)
	{
		v->data = malloc(capacity * item_size);
		if(!v->data)
		{
			free(v);
			return NULL;
		}

		if(growth_rate == 0)
			v->growth_rate = 1.6f;
		else
			v->growth_rate = growth_rate;

		v->size = 0;
		v->capacity = capacity;
		v->item_size = item_size;
	}
	return v;
}

void vector_destroy(vector_t *v)
{
	free(v->data);
	free(v);
}

int vector_push(vector_t *v, const void *i)
{
	if(v->size == v->capacity)
	{
		size_t new_capacity = v->size * v->growth_rate + 1;
		void *new_data = realloc(v->data, new_capacity * v->item_size);

		if(!new_data)
			return -1;

		v->data = new_data;
		v->capacity = new_capacity;
	}

	memcpy((char*)v->data + v->size * v->item_size, i, v->item_size);
	v->size++;
	return v->size;
}

void vector_pop(vector_t *, void *);

void *vector_get(vector_t *v, size_t index)
{
	assert(index >= 0 && index <= v->size);
	return (char*)v->data+index*v->item_size;
}

int vector_join(vector_t *dest, vector_t *src)
{
	assert(dest->item_size == src->item_size);

	if(dest->capacity < dest->size + src->size)
	{
		size_t new_capacity = dest->size + src->size;
		void *new_data = realloc(dest->data, new_capacity * dest->item_size);

		if(!new_data)
			return -1;

		dest->data = new_data;
		dest->capacity = new_capacity;
	}
	memcpy(&dest->data[dest->size], src->data, src->size);

	return 0;
}
