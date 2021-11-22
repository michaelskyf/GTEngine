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
#include <GTEngine/lib.h>
#include <string.h>
#include <stdio.h>

const char *basename(const char *path)
{
	char *base = strrchr(path, '/');
	return base ? base+1 : path;
}

char *dirname(char *path)
{
	char *base = strrchr(path, '/');
	if(base)
		*base='\0';
	return path;
}
