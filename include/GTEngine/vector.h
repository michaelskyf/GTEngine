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
