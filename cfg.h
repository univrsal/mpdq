#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <util.h>
#include <X11/Xlib.h>

#ifndef CFG_H
#define CFG_H

struct _Config
{	
	float icon_scale;
	int   root_window_song;
};

typedef struct _Config Config;

FILE* open_file_rw(const char* path);

void close_file(FILE* file);

char* read_line(FILE* file);

Config* create_or_open_cfg(char* path);

#endif /* CFG_H */