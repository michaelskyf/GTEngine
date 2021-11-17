#include <string.h>

const char *dirname(char *path)
{
	char *base = strrchr(path, '/');
	if(base)
		*base='\0';
	return path;
}

const char *basename(const char *path)
{
	char *base = strrchr(path, '/');
	return base ? base+1 : path;
}
