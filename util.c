/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include "util.h"

void _log(char* c, int stop)
{
	if (debug)
		printf(c);
	if (stop) 
		exit(EXIT_FAILURE);
}

char* append(char* a, char* b)
{
    char* out;
    if (a == NULL || b == NULL)
        return NULL;
    if((out = (char *)malloc(strlen(a) + strlen(b) + 1)) != NULL)
    {
       strcpy(out, a);
       strcat(out, b);
    }
    else
        _log("Not enough memory to concat strings!", 1);
    a = NULL;
    b = NULL;
    return out;
}

int startsWith(const char* str, const char* pre)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? 0 : strncmp(pre, str, lenpre) == 0;
}

void scale_point(float scale, XPoint* p)
{
    p->x = p->x * scale;
    p->y = p->y * scale;
}

void scale_rect(float scale, Rect* r)
{
    r->x = r->x * scale;
    r->y = r->y * scale;
    r->w = r->w * scale;
    r->h = r->h * scale;
}

void free_rect(Rect* r)
{
    r->x = 0;
    r->y = 0;
    r->w = 0;
    r->h = 0;
}