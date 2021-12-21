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

#include <GLES2/gl2.h>
#include <GTEngine/output.h>
#include <glad/glad.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

static int is_tty_stdout;
static int is_tty_stderr;

int print_setup(void)
{
	is_tty_stdout = isatty(fileno(stdout));
	is_tty_stderr = isatty(fileno(stderr));
	return 0;
}

int print_core(FILE *stream, const char *color, const char *fmt, ...)
{
	int ret;
	int print_color = 0;

	va_list args;
	va_start(args, fmt);

	if(color)
	{
		if(stream == stdout)
			print_color = is_tty_stdout;
		else if(stream == stderr)
			print_color = is_tty_stderr;
	}

	// If printing to tty, add color
	if(print_color)
	{
		size_t color_offset = sizeof(TERMINAL_COLOR_BLUE) - 1; // Assuming that all colors are equal in length
		size_t color_reset_offset = sizeof(TERMINAL_COLOR_RESET);
		size_t fmt_offset = strlen(fmt);
		int add_newline = 0;

		// If the last charracter of fmt is '\n' (not including '\0')
		// we need to move that '\n' to the end of new_fmt
		// otherwise it can print improperly
		if(fmt[fmt_offset-1] == '\n')
		{
			fmt_offset--;
			color_reset_offset--; // since instead of '\0' end of new_fmt will be '\n','\0'
			add_newline = 2; // we need space for '\n' and '\0'
		}

		char *new_fmt = malloc(color_offset + fmt_offset + color_reset_offset + add_newline);
		char *new_fmt_start = new_fmt;

		memcpy(new_fmt, color, color_offset);
		new_fmt += color_offset;

		memcpy(new_fmt, fmt, fmt_offset);
		new_fmt += fmt_offset;

		// Important: since we call sizeof() insted of strlen(), memcpy will also copy '\0' (if add_newline != 0)
		memcpy(new_fmt, TERMINAL_COLOR_RESET, color_reset_offset);

		if(add_newline)
		{
			new_fmt += color_reset_offset;
			new_fmt[0] = '\n';
			new_fmt[1] = '\0';
		}

		ret = vfprintf(stream, new_fmt_start, args);
		free(new_fmt_start);
	}
	else
	{
		ret = vfprintf(stream, fmt, args);
	}

	return ret;
}

int print_opengl_error(void)
{
	GLenum e;
	int flag = 0;
	while((e = glGetError()) != GL_NO_ERROR)
	{
		LOGE("OpenGL: %#05x", e);
		flag++;
	}
	return flag;
}
