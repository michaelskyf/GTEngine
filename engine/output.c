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

static int is_tty_stdout;
static int is_tty_stderr;

int print_setup(void)
{
	is_tty_stdout = isatty(fileno(stdout));
	is_tty_stderr = isatty(fileno(stderr));
	return 0;
}

int print_core(FILE *stream, const char *color, const char *prefix, const char *fmt, ...)
{
	int print_color = 0;

	va_list arg;
	va_start(arg, fmt);

	if(color)
	{
		if(stream == stdout)
			print_color = is_tty_stdout;
		if(stream == stderr)
			print_color = is_tty_stderr;
	}

	if(print_color)
	{
		fprintf(stream, "%s", color);
	}

	if(prefix)
	{
		fprintf(stream, "%s ", prefix);
	}

	int ret = vfprintf(stream, fmt, arg);
	putc('\n', stream);

	if(print_color)
	{
		fprintf(stream, "%s", TERMINAL_COLOR_RESET);
	}

	return ret;
}
