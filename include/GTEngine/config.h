/*
  * This file contains various variables and default values
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
