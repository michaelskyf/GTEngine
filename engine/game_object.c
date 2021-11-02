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

#include <GTEngine/game_object.h>
#include <GTEngine/output.h>

game_object_t *game_object_create(vec3 pos, _Bool destroyed)
{
	LOGD("TODO");
	return NULL;
}

void game_object_destroy(game_object_t *game_object)
{
	model_destroy(game_object->model);
}
