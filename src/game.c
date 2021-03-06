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
#include <GTEngine/shader.h>
#include <GTEngine/game_object.h>
#include <lib/selog.h>
#include <GTEngine/engine.h>

int game_setup(void)
{
	model_t *model = model_load("data/objects/Earth_1_12756.glb");
	if(model){
		game_object_create(model, (vec3){0,0,0}, (vec3){0.01,0.01,0.01});
	}

	return 0;
}

void game_exit(void)
{

}

void game_update(void)
{
}
