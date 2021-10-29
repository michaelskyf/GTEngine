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
		size_t color_offset = strlen(color);
		size_t fmt_offset = strlen(fmt);
		char *new_fmt = malloc(color_offset + fmt_offset + sizeof(TERMINAL_COLOR_RESET) + 1);

		memcpy(new_fmt, color, color_offset);
		memcpy(new_fmt + color_offset, fmt, fmt_offset);
		memcpy(new_fmt + color_offset + fmt_offset, TERMINAL_COLOR_RESET, sizeof(TERMINAL_COLOR_RESET));
		new_fmt[color_offset + fmt_offset + sizeof(TERMINAL_COLOR_RESET)] = '\0';

		ret = vfprintf(stream, new_fmt, args);
		free(new_fmt);
	}
	else
	{
		ret = vfprintf(stream, fmt, args);
	}

	return ret;
}
