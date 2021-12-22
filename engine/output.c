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

#include <GTEngine/output.h>
#include <glad/glad.h>
#include <stdio.h>
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

int print_core(FILE *stream, const char *color, const char *fmt, va_list args)
{
	int ret;

	if((stream == stdout && !is_tty_stdout) || (stream == stderr && !is_tty_stderr))
	{
		ret = vfprintf(stream, fmt, args);
	}
	else
	{
		char buffer[2048];
		vsnprintf(buffer, sizeof(buffer), fmt, args);

		ret = fprintf(stream, "%s%s%s", color, buffer, TERMINAL_COLOR_RESET);
	}
	return ret;
}

int vprint_log(const char *fmt, va_list args)
{
	return print_core(stdout, TERMINAL_COLOR_RESET, fmt, args);
}

int vprint_info(const char *fmt, va_list args)
{
	return print_core(stdout, TERMINAL_COLOR_BLUE, fmt, args);
}

int vprint_debug(const char *fmt, va_list args)
{
	return print_core(stdout, TERMINAL_COLOR_GREEN, fmt, args);
}

int vprint_warning(const char *fmt, va_list args)
{
	return print_core(stderr, TERMINAL_COLOR_YELLOW, fmt, args);
}

int vprint_error(const char *fmt, va_list args)
{
	return print_core(stderr, TERMINAL_COLOR_RED, fmt, args);
}

int print_log(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	return vprint_log(fmt, args);
}

int print_info(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	return vprint_info(fmt, args);
}

int print_debug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	return vprint_debug(fmt, args);
}

int print_warning(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	return vprint_warning(fmt, args);
}

int print_error(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	return vprint_error(fmt, args);
}

int print_opengl_errors(const char *fmt, ...)
{
	GLenum e;
	int ret = 0;
	va_list args;
	va_start(args, fmt);

	while((e = glGetError()) != GL_NO_ERROR)
	{
		print_error("OpenGL error (%#05x)\n", e);
		ret++;
	}
	if(ret)
		vprint_error(fmt, args);
	return ret;
}
