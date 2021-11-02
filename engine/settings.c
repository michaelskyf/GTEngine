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

#include <GTEngine/settings.h>
#include <GTEngine/output.h>
#include <GTEngine/fileio.h>

#include <stdlib.h>
#include <string.h>

static void settings_parse(settings_t *, const char *);

settings_t *settings_create(void)
{
	settings_t *set = malloc(sizeof(settings_t));

	memcpy(set, &default_settings, sizeof(settings_t));

	return set;
}

void settings_destroy(settings_t *set)
{
	free(set);
}

int settings_read(const char *path, settings_t *set)
{
	char *file = file_read(path);

	if(!file)
		return -1;

	settings_parse(set, file);

	free(file);
	return 0;
}

static void settings_parse(settings_t *set, const char *file)
{
	LOGW("Parsing settings is not supported yet");
}
