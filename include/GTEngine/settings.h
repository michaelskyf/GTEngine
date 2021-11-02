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

#ifndef GTE_SETTINGS_H
#define GTE_SETTINGS_H

typedef struct {

	/* Display-related */
	unsigned int width, height;
	_Bool fullscreen;
	_Bool resizable;

	/* Camera-related */
	float viewport;

	/* Graphics quality related */
} settings_t;

static const settings_t default_settings = {
	/* Display */
	.width = 800,
	.height = 600,
	.fullscreen = 0,
	.resizable = 1,

	/* Camera */
	.viewport = 45.0f,

	/* Quality */
};

settings_t *settings_create(void);
void settings_destroy(settings_t *);

// Reads settings from given file
int settings_read(const char *, settings_t *);
// Writes settings into given file
int settings_write(const char *, settings_t *);

#endif
