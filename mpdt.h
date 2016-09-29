/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include <X11/Xlib.h>
#include <string.h>
#include <mpd/client.h>
#include <mpd/stats.h>
#include <util.h>
#include <cfg.h>

/* Vars */

Window tray_windows[3];

GC gc[3]; // Graphic context for the icons

Display *display;

XEvent event;

int screen;

uint32_t tray_width = 32, tray_height = 32;

XPoint play_path[] = { { 3, 2 }, { 15, 8 }, { 3 , 14 }, { 3, 2 } }; // Path of the 'Play' triangle

XPoint back_path[] = { { 2, 8 }, { 14, 2 }, { 14, 14 }, { 2, 8 } }; // Path of the 'Last Song' triangle

int npathpoints = 4;

/* Methods */

void initX(Config* cfg);

void send_message(Display* dpy, Window w, long message, long data1, long data2, long data3);

void tray_window_event(int btn, int state, struct mpd_connection* conn);

void create_tray_icons(void);

void draw_tray(int state);

void destroy_tray_icons(void);

int handle_events(void);