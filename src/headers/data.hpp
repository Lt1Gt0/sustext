#ifndef __DATA_HPP
#define __DATA_HPP

#ifndef __DEFINES_HPP
	#include "defines.hpp"
#endif // __DEFINES_HPP

typedef struct erow{
	int 	size;
	int		rsize;
	char* 	chars;
	char* 	render;
}erow;

struct editorConfig{
	int 	cx, cy;
	int 	rowOff;
	int 	colOff;
	int 	screenRows;
	int 	screenCols;
	int 	numrows;
	erow* 	row;
	struct termios orig_termios;
};

static editorConfig E;
#endif // __DATA_HPP
