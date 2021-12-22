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
#ifndef GTE_OUTPUT_H
#define GTE_OUTPUT_H

#include <stdio.h>

#define TERMINAL_COLOR_RED 	"\033[0;31m"
#define TERMINAL_COLOR_GREEN 	"\033[0;32m"
#define TERMINAL_COLOR_YELLOW 	"\033[0;33m"
#define TERMINAL_COLOR_BLUE 	"\033[0;34m"
#define TERMINAL_COLOR_PURPLE 	"\033[0;35m"
#define TERMINAL_COLOR_CYAN 	"\033[0;36m"
#define TERMINAL_COLOR_WHITE 	"\033[0;37m"

#define TERMINAL_COLOR_RESET	"\033[0m"


int print_setup(void);

int print_core(FILE *stream, const char *color, const char *fmt, va_list args);
__attribute__((format(printf, 1, 2)))
int print_log(const char *fmt, ...);
__attribute__((format(printf, 1, 2)))
int print_info(const char *fmt, ...);
__attribute__((format(printf, 1, 2)))
int print_debug(const char *fmt, ...);
__attribute__((format(printf, 1, 2)))
int print_warning(const char *fmt, ...);
__attribute__((format(printf, 1, 2)))
int print_error(const char *fmt, ...);
__attribute__((format(printf, 1, 2)))
int print_opengl_errors(const char *fmt, ...);

#endif
