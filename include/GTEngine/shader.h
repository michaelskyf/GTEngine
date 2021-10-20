#ifndef GTE_SHADER_H
#define GTE_SHADER_H

typedef struct shader_t {
	unsigned int id;
} shader_t;

// geometry shader may be NULL
shader_t *shader_create(const char *v_path, const char *f_path, const char *g_path);
void shader_destroy(shader_t *);

#endif
