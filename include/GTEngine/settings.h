#ifndef GTE_SETTINGS_H
#define GTE_SETTINGS_H

struct settings {

	/* Display-related */
	unsigned int width, height;
	_Bool fullscreen;
	_Bool resizable;

	/* Camera-related */
	float viewport;

	/* Graphics quality related */
};

// Reads settings from given file
int settings_read(const char *, struct settings *);
// Writes settings into given file
int settings_write(const char *, struct settings *);

#endif
