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

#ifndef GTE_ENGINE_H
#define GTE_ENGINE_H

#include "settings.h"
#include "output.h"
#include "vector.h"

struct engine_variables {
	struct settings *settings;

	Vector *game_objects;
	Vector *shaders;
};

extern struct engine_variables evars;


int program_setup(void);
void program_exit(void);
void program_update(void);

#endif
