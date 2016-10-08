/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
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
	unsigned int root_window_song;
	unsigned int song_to_text_file;
	char* file_path;
	unsigned int delay;
	int icon_color;
	unsigned int max_song_length;
	int volume_timeout;
};

typedef struct _Config Config;

FILE* open_file_rw(const char* path);

void close_file(FILE* file);

char* read_line(FILE* file);

Config* create_or_open_cfg(char* path);

#endif /* CFG_H */