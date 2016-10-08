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
#include <cfg.h>
#include <util.h>

/* Vars */

Window tray_windows[3];

GC gc[3]; // Graphic context for the icons

Display *display; // The X Display

XEvent event; // For Tray event Handling

int screen; // X Screen ID

int npathpoints = 3; // Amount of poins in the triangle path

uint32_t tray_width = 32, tray_height = 32;

XPoint play_path[] = { { 3, 2 }, { 15, 8 }, { 3 , 14 } }; // Path of the 'Play' triangle

XPoint back_path[] = { { 2, 8 }, { 14, 2 }, { 14, 14 } }; // Path of the 'Last Song' triangle

Rect back_rect = { 0,  3, 2, 11 };

Rect next_rect = { 15, 3, 2, 11 };

Rect pause_rect1 = { 4,  3, 3, 11 };

Rect pause_rect2 = { 10, 3, 3, 11 };

/* Methods */

void initX(Config* cfg);

void send_message(Display* dpy, Window w, long message, long data1, long data2, long data3);

void tray_window_event(int btn, int state, struct mpd_status* status, struct mpd_connection* conn);

void create_tray_icons(void);

void draw_tray(int state, int icon_color);

void destroy_tray_icons(void);

int handle_events(void);

void change_volume(int v, struct mpd_status* status, struct mpd_connection* conn);