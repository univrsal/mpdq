/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

static char* version = "1.0";
static int debug = 1;
extern char* volume;
extern int cycles;

struct _Rect
{	
	int x, y, w, h;
};

typedef struct _Rect Rect;

void die(char* c, int stop);

void _log(char* c);

char* append(char* a, char* b);

int startsWith(const char* str, const char* pre);

void scale_point(float scale, XPoint* p);

void scale_rect(float scale, Rect* r);

void free_rect(Rect* r);

char* itoc(int i);

#endif /* UTIL_H */