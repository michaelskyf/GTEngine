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

#include <stddef.h> /* for size_t */
typedef struct Vector Vector;

/* Vector creation and destruction */
Vector *vector_create(size_t capacity, size_t itemsize);
void vector_destroy(Vector *v);

/* Vector access operations */
int vector_push(Vector *v, const void *item);
void vector_pop(Vector *v, void *item);
void* vector_get_element(Vector *v, size_t index);
void* vector_start(Vector *v);
void* vector_end(Vector *v);
int vector_create_element(Vector *v);

/* Size operations */
size_t vector_item_size(const Vector *v);
size_t vector_size(const Vector *v);
size_t vector_capacity(const Vector *v);
int vector_set_size(Vector *v, size_t size);
int vector_set_capacity(Vector *v, size_t size);

/* custom functions */
Vector *vector_clone(Vector *v);

#endif
