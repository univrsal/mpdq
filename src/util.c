/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include "util.h"

char* version = "3.0";
char* volume = NULL;
int cycles = 0;
int debug = 1;

void die(char* c, int stop)
{
	if (debug)
		printf(c);
	if (stop)
		exit(EXIT_FAILURE);
}

void _log(char* c)
{
    die(c, 0);
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
        die("Not enough memory to concat strings!", 1);
    a = NULL;
    b = NULL;
    return out;
}

int starts_with(const char* str, const char* pre)
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

void shift_point(int x, int y, XPoint* p)
{
    p->x += x;
    p->y += y;
}

void shift_rect(int x, int y, Rect* r)
{
    r->x += x;
    r->y += y;
}

void free_rect(Rect* r)
{
    r->x = 0;
    r->y = 0;
    r->w = 0;
    r->h = 0;
}

char* itoc(int i)
{
     char *buffer;

     buffer = malloc(10);
     if(buffer)
        snprintf(buffer, 9, " [%d%]", i);
     else
        printf("Not enough ram..\n");
     return buffer;
}
