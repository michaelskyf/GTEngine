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
