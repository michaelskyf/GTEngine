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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <GTEngine/vector.h>

static const size_t StartSize = 5;
static const float GrowthRate = 1.6f; /* Geometric growth of vector capacity */

typedef struct Vector {

	void *data;		/* pointer to vector elements */
	size_t capacity;	/* current reserved memory for vector */
	size_t size;		/* current size of vector (number of stored items) */
	size_t itemsize;	/* size of each item */
} Vector;

Vector *vector_create(size_t capacity, size_t itemsize)
/* Initialise a vector with the specified capacity for items of size itemsize */
{
	Vector *v = (Vector*)malloc(sizeof(Vector));
	if (v) {
		v->data = malloc(capacity*itemsize);
		v->capacity = (v->data == NULL) ? 0 : capacity;
		v->size = 0;
		v->itemsize = itemsize;
	}
	return v;
}

void vector_destroy(Vector *v)
/* Release memory owned by vector */
{
	free(v->data);
	free(v);
}

int vector_push(Vector *v, const void *item)
/* Add element to back of vector. Return index of new element if successful and -1 if not */
{
	/* If out-of-space, allocate more */
	if (v->size == v->capacity) {
		size_t newsize = (v->capacity == 0) ? StartSize : (size_t)(v->capacity*GrowthRate + 1.0);
		void *p = realloc(v->data, v->itemsize*newsize);
		if (p == NULL)
			return -1;

		v->capacity = newsize; /* allocate succeds, update data-structure */
		v->data = p;
	}

	/* We have enough room */
	memcpy((char*)v->data + v->size * v->itemsize, item, v->itemsize);
	return v->size++;
}

void vector_pop(Vector *v, void *item)
/* Return element from back of vector and remove it from the vector */
{
	assert(v->size > 0);
	--v->size;
	memcpy(item, (char*)v->data + v->size * v->itemsize, v->itemsize);
}

void* vector_get_element(Vector *v, size_t index)
/* Return pointer to the element at the specified index */
{
	assert(index >= 0 && index < v->size);
	return (char*)v->data + index*v->itemsize;
}

/* Return pointer to beginning of the array */
void* vector_start(Vector *v) { return v->data; }

/* Return pointer to beginning of the array */
void* vector_end(Vector *v) { return (char*)v->data + v->size*v->itemsize; }

/* Inquire afer size of vector item */
size_t vector_item_size(const Vector *v) { return v->itemsize; }

/* Inquire afer vector size and capacity */
size_t vector_size(const Vector *v) { return v->size; }
size_t vector_capacity(const Vector *v) { return v->capacity; }

int vector_set_size(Vector *v, size_t size)
/* Set vector size. Return 0 if succesful, -1 if fails */
{
	if (size > v->capacity) {
		void *p = realloc(v->data, size*v->itemsize);
		if (p == NULL)
			return -1;

		v->capacity = size; /* Allocate succeds, update data-structure */
		v->data = p;
	}

	v->size = size;
	return 0;
}

int vector_set_capacity(Vector *v, size_t size)
/* Shrink or grow allocated memory reserve for array. Return 0 if successful, -1 if fails */
{
	if (size != v->capacity) {
		void *p = realloc(v->data, size*v->itemsize);
		if (p == NULL && size > 0)
			return -1;

		v->capacity = size;
		v->data = p;
	}

	if (size < v->size)
		v->size = size;
	return 0;
}

int vector_create_element(Vector *v)
/* Create an empty element. Return element ID if succesful, -1 if fails */
{
	void *p = calloc(1, v->itemsize);
	if (p == NULL)
		return -1;
	int ret = vector_push(v, p);
	free(p);
	return ret;
}

Vector *vector_clone(Vector *v)
{
	Vector *newv = malloc(sizeof(Vector));
	memcpy(newv, v, sizeof(Vector));

	size_t cap = vector_capacity(v);
	void *data = malloc(cap);
	memcpy(data, v->data, cap);

	newv->data = data;

	return newv;
}
