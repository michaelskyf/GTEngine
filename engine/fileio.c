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
#include <GTEngine/fileio.h>
#include <GTEngine/output.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *file_read(const char *path)
{
	char *file = NULL;
	size_t fsize;
	FILE *fp = fopen(path, "r");

	if(!fp)
	{
		LOGE("Failed to open %s for reading(%d): %s", path, errno, strerror(errno));
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	file = malloc(fsize+1);
	file[fsize] = '\0';

	size_t read = fread(file, sizeof(char), fsize, fp);

	fclose(fp);

	if (fsize != read)
	{
		LOGE("Failed to read %s", path);
		free(file);
		return NULL;
	}

	return file;
}

size_t file_write(const char *path, const char *content)
{
	size_t written = 0;
	size_t fsize = strlen(content);
	FILE *fp = fopen(path, "w");

	if(!fp)
	{
		LOGE("Failed to open %s for writing(%d): %s", path, errno, strerror(errno));
		return -1;
	}

	written = fwrite(content, sizeof(char), fsize, fp);

	fclose(fp);

	if(written != fsize)
	{
		LOGE("Failed to write to %s", path);
		return -1;
	}

	return written;
}
