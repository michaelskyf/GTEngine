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

struct settings {

	/* Display-related */
	unsigned int width, height;
	_Bool fullscreen;
	_Bool resizable;

	/* Camera-related */
	float viewport;

	/* Graphics quality related */
};

// Reads settings from given file
int settings_read(const char *, struct settings *);
// Writes settings into given file
int settings_write(const char *, struct settings *);

#endif
