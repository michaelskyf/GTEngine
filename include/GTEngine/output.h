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

#define TERMINAL_COLOR_RED 	"\e[0;31m"
#define TERMINAL_COLOR_GREEN 	"\e[0;32m"
#define TERMINAL_COLOR_YELLOW 	"\e[0;33m"
#define TERMINAL_COLOR_BLUE 	"\e[0;34m"
#define TERMINAL_COLOR_PURPLE 	"\e[0;35m"
#define TERMINAL_COLOR_CYAN 	"\e[0;36m"
#define TERMINAL_COLOR_WHITE 	"\e[0;37m"

#define TERMINAL_COLOR_RESET	"\e[0m"


#define LOG(fmt, ...)  print_core(stdout, NULL, fmt "\n", ##__VA_ARGS__)

#define LOGI(fmt, ...) print_core(stdout, TERMINAL_COLOR_BLUE, "[INF] " fmt "\n", ##__VA_ARGS__)
#define LOGD(fmt, ...) print_core(stdout, TERMINAL_COLOR_GREEN, "[DBG] %s:%d->%s() " fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define LOGW(fmt, ...) print_core(stdout, TERMINAL_COLOR_YELLOW, "[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) print_core(stderr, TERMINAL_COLOR_RED, "[ERR] %s(): " fmt "\n", __func__, ##__VA_ARGS__)

int print_setup(void);

int print_core(FILE *stream, const char *color, const char *fmt, ...);

#endif
