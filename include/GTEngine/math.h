#ifndef GTE_MATH_H
#define GTE_MATH_H

typedef struct {
	union {
		struct {float x,y,z;};
		float v[3];
	};
} vec3;

#endif
