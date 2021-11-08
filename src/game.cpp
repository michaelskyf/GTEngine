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
extern "C"
{
#include <GTEngine/output.h>
// For testing only
#include <GTEngine/model.h>
}

class Model
{
	public:
		Model(const char *path)
		{
			model = model_load(path);
		}
		~Model(void)
		{
			model_destroy(model);
		}
		void Draw()
		{
			model_draw(model);
		}

	private:
		model_t *model;
};

Model *t;

extern "C" int game_setup(void)
{
	t = new Model("data/backpack.obj");

	return 0;
}

extern "C" void game_exit(void)
{

}

extern "C" void game_update(void)
{
	t->Draw();
}
