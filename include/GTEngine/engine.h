#ifndef GTE_ENGINE_H
#define GTE_ENGINE_H

#include <GTEngine/settings.h>
#include "macros.h"

struct engine_variables {
	struct settings settings;
};

extern struct engine_variables evars;


int program_setup(void);
void program_update(void);

#endif
