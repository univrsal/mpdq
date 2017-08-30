/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <src/util.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifndef CFG_H
#define CFG_H

#define true 1
#define false 0

typedef int _bool;

struct _Config
{
	_bool enable_icons;
	_bool enable_hotkeys;
	_bool song_to_root_window;
	_bool song_to_text_file;
	_bool reverse_icons;

	float icon_scale;
	char* file_path;
	char* title_text;
	unsigned int delay;
	int icon_color;
	unsigned int max_song_length;
	int xOffset, yOffset;
	int volume_timeout;


	int key_next;
	int key_prev;
	int key_pause;
	unsigned int key_mod;
};

typedef struct _Config Config;

FILE* open_file_rw(const char* path);

void close_file(FILE* file);

char* read_line(FILE* file);

Config* create_or_open_cfg(char* path);

#endif /* CFG_H */
