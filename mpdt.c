/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include <mpdt.h>
#include <stdio.h>
#define SYSTEM_TRAY_REQUEST_DOCK 0
#define BTN_NEXT 0
#define BTN_PLAY 1
#define BTN_PREV 2

void initX(Config* cfg)
{
    XInitThreads();
    if((display = XOpenDisplay(NULL)) == NULL) {
        _log("Cannot open display", 1);
    }

    screen = DefaultScreen(display);

    for (int i = 0; i < npathpoints; i++)
        scale_point(cfg->icon_scale, &play_path[i]);

    for (int i = 0; i < npathpoints; i++)
        scale_point(cfg->icon_scale, &back_path[i]);

    create_tray_icons();
}

void send_message(Display* dpy, Window w, long message, long data1, long data2, long data3)
{
    XEvent ev;

    memset(&ev, 0, sizeof(ev));
    ev.xclient.type = ClientMessage;
    ev.xclient.window = w;
    ev.xclient.message_type = XInternAtom (dpy, "_NET_SYSTEM_TRAY_OPCODE", False );
    ev.xclient.format = 32;
    ev.xclient.data.l[0] = CurrentTime;
    ev.xclient.data.l[1] = message;
    ev.xclient.data.l[2] = data1;
    ev.xclient.data.l[3] = data2;
    ev.xclient.data.l[4] = data3;

    XSendEvent(dpy, w, False, NoEventMask, &ev);
    XSync(dpy, False);
}

void tray_window_event(int btn, int state, struct mpd_connection* conn) {
    switch (btn)
    {
        case BTN_NEXT:
            mpd_run_next(conn);
            break;
        case BTN_PLAY:
            if (state > 1)
            {
                mpd_run_toggle_pause(conn);
            }
            break;
        case BTN_PREV:
            mpd_run_previous(conn);
    }
}

void create_tray_icons(void)
{
    for (int i = 0; i < 3; i++)
    {
        tray_windows[i] = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, tray_width, tray_height, 0, BlackPixel(display, screen), WhitePixel(display, screen));
        gc[i] = XCreateGC(display, tray_windows[i], 0, 0);  
        XSelectInput(display, tray_windows[i], ButtonPress);
        send_message(display, XGetSelectionOwner(display, XInternAtom(display, "_NET_SYSTEM_TRAY_S0", False)), SYSTEM_TRAY_REQUEST_DOCK, tray_windows[i], 0, 0);
        
        Window root;
        int32_t x_r = 0, y_r = 0;
        uint32_t border_r = 0, xwin_depth_r = 0;
        
        XMoveResizeWindow(display, tray_windows[i], x_r, y_r, 32, 32);
        XGetGeometry(display, tray_windows[i], &root, &x_r, &y_r, &tray_width, &tray_height, &border_r, &xwin_depth_r);
    }
}

void draw_tray(int state)
{
    XSetForeground(display, gc[1], 0x222222); // Clean up old play/pause icon
    XFillRectangle(display, tray_windows[1], gc[1], 0, 0, 20, 20);

    for (int i = 0; i < 3; i++)
    {
        XSetForeground(display, gc[i], 0xbbbbbb);

        switch (i)
        {
            case BTN_NEXT:
                XFillPolygon(display, tray_windows[i],   gc[i], play_path, npathpoints, Complex, CoordModeOrigin); // Play Triangle
                XFillRectangle(display, tray_windows[i], gc[i], 15, 3, 2, 11);
                break;
            case BTN_PLAY:
                if (state != 2)
                    XFillPolygon(display, tray_windows[i], gc[i], play_path, npathpoints, Complex, CoordModeOrigin); // Play Icon
                else // Pause
                {
                    XFillRectangle(display, tray_windows[i], gc[i], 4, 3, 3, 11);
                    XFillRectangle(display, tray_windows[i], gc[i], 10, 3, 3, 11);
                }
                break;
            case BTN_PREV:
                XFillPolygon(display, tray_windows[i], gc[i], back_path, npathpoints, Complex, CoordModeOrigin);
                XFillRectangle(display, tray_windows[i], gc[i], 0, 3, 2, 11);
        }
    }
}

int handle_events(void)
{
    while(XPending(display))
    {
        XNextEvent(display, &event);
        for (int i = 0; i < 3; i++)
        {
            if (event.xany.window == tray_windows[i])
            {
                return i;
            }
        }
    }
    return -1;
}

void destroy_tray_icons(void)
{
    for (int i = 0; i < 3; i++)
        XDestroyWindow(display, tray_windows[i]);

    for (int i = 0; i < 3; i++)
         XFreeGC(display, gc[i]);
    
    XCloseDisplay(display);
}