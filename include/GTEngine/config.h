/*
  * This file contains various variables and default values

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

#ifndef GTE_CONFIG_H
#define GTE_CONFIG_H

#include <GTEngine/settings.h>

static const char *title = "GLTest";
static const char *settings_path = "settings.conf";

static const int opengl_version_major = 2;
static const int opengl_version_minor = 0;

static const struct settings settings_default = {
	/* Display */
	.width = 800,
	.height = 600,
	.fullscreen = 0,
	.resizable = 1,

	/* Camera */
	.viewport = 45.0f,

	/* Quality */
};

#endif
