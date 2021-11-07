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
#include "GTEngine/shader.h"
#include <GTEngine/output.h>
// For testing only
#include <GTEngine/mesh.h>
#include <GTEngine/vector.h>
#include <GTEngine/model.h>

model_t *model;

int game_setup(void)
{
	model = model_load("data/backpack.obj");

	return 0;
}

void game_exit(void)
{

}

void game_update(void)
{
	model_draw(model);
}
