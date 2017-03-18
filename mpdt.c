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
#define BTN_SCROLLUP 3
#define BTN_SCROLLDOWN 4

#define EVENT_NEXT 0
#define EVENT_PREV 2

void initX(Config* cfg)
{
    XInitThreads();
    if((display = XOpenDisplay(NULL)) == NULL) {
        die("Cannot open display", 1);
    }

    screen = DefaultScreen(display);

    root_window = DefaultRootWindow(display);

    for (int i = 0; i < npathpoints; i++)
        scale_point(cfg->icon_scale, &play_path[i]);

    for (int i = 0; i < npathpoints; i++)
        scale_point(cfg->icon_scale, &back_path[i]);

    create_tray_icons(cfg->reverse);
    
    setup_keybinds(cfg);

    scale_rect(cfg->icon_scale, &next_rect);
    scale_rect(cfg->icon_scale, &back_rect);
    scale_rect(cfg->icon_scale, &pause_rect1);
    scale_rect(cfg->icon_scale, &pause_rect2);
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

void tray_window_event(int btn, int state, struct mpd_status* status, struct mpd_connection* conn) {
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
            break;
        case BTN_SCROLLDOWN:
            change_volume(-1, status, conn);
            break;
        case BTN_SCROLLUP:
            change_volume(1, status, conn);
            break;
    }
}

void create_tray_icons(int reverse)
{

    XVisualInfo vinfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);

    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(display, root_window, vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0;

    if (reverse)
    {
        for (int i = 2; i >= 0; i--)
        {
            tray_windows[i] = XCreateWindow(display, RootWindow(display, screen), 0, 0, tray_width, tray_height, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &attr);
            //XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, tray_width, tray_height, 0, BlackPixel(display, screen), WhitePixel(display, screen));
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
    else
    {
        for (int i = 0; i < 3; i++)
        {
            tray_windows[i] = XCreateWindow(display, RootWindow(display, screen), 0, 0, tray_width, tray_height, 0, vinfo.depth, InputOutput, vinfo.visual, CWColormap | CWBorderPixel | CWBackPixel, &attr);
            //XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, tray_width, tray_height, 0, BlackPixel(display, screen), WhitePixel(display, screen));
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

}

void draw_tray(int state, int icon_color)
{ 
    XClearWindow(display, tray_windows[1]);

    for (int i = 0; i < 3; i++)
    {
        XSetForeground(display, gc[i], icon_color);
        switch (i)
        {
            case BTN_NEXT:
                XFillPolygon(display, tray_windows[i],   gc[i], play_path, npathpoints, Complex, CoordModeOrigin); // Play Triangle
                XFillRectangle(display, tray_windows[i], gc[i], next_rect.x, next_rect.y, next_rect.w, next_rect.h);
                break;
            case BTN_PLAY:
                if (state != 2)
                    XFillPolygon(display, tray_windows[i], gc[i], play_path, npathpoints, Complex, CoordModeOrigin); // Play Icon
                else // Pause
                {
                    XFillRectangle(display, tray_windows[i], gc[i], pause_rect1.x, pause_rect1.y, pause_rect1.w,  pause_rect1.h);
                    XFillRectangle(display, tray_windows[i], gc[i], pause_rect2.x, pause_rect2.y, pause_rect2.w,  pause_rect2.h);
                }
                break;
            case BTN_PREV:
                XFillPolygon(display, tray_windows[i], gc[i], back_path, npathpoints, Complex, CoordModeOrigin);
                XFillRectangle(display, tray_windows[i], gc[i], back_rect.x, back_rect.y, back_rect.w, back_rect.h);
        }
    }
}

int handle_events(void)
{
    while(XPending(display))
    {
        XNextEvent(display, &event);

        switch(event.type)
        {
            case ButtonPress:
                if (event.xbutton.button == Button1) // Clicked on traybar -> execute button action
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (event.xany.window == tray_windows[i])
                        {
                            return i;
                        }
                    }  
                }
                else if (event.xbutton.button == Button4)
                    return BTN_SCROLLUP;
                else if (event.xbutton.button == Button5)
                    return BTN_SCROLLDOWN;
                break;
            case KeyPress:
                printf("Keypressed! KeyCode: %i\n", event.xkey.keycode);

                if (event.xkey.keycode == key_prev) {
                    printf("Hotkey for previous song pressed!\n");
                    return BTN_PREV;
                } 
                else if (event.xkey.keycode == key_next)
                {
                    printf("Hotkey for next song pressed!\n");
                    return BTN_NEXT;
                }
                else if (event.xkey.keycode == key_pause)
                {
                    printf("Hotkey for pausing pressed\n");
                    return BTN_PLAY;
                }
                break;
        }
    }
    return -1;
}

void change_volume(int v, struct mpd_status* status, struct mpd_connection* conn)
{
    int d = mpd_status_get_volume(status);
    d = d + v <= 100 ? (d + v >= 0 ? d + v : 0) : 100;

    volume = itoc(d);
    mpd_run_set_volume(conn, d);
    cycles = 0;
}

void setup_keybinds(Config* cfg)
{
    int pointer_mode = GrabModeAsync;
    int keyboard_mode = GrabModeAsync;
    
    XGrabKey(display, cfg->key_next, cfg->key_mod, root_window, False, pointer_mode, keyboard_mode);
    XGrabKey(display, cfg->key_prev, cfg->key_mod, root_window, False, pointer_mode, keyboard_mode);
    XGrabKey(display, cfg->key_pause, cfg->key_mod, root_window, False, pointer_mode, keyboard_mode);

    // Local copies for event handling
    key_next = cfg->key_next;
    key_prev = cfg->key_prev;
    key_pause = cfg->key_pause;

    XSelectInput(display, root_window, KeyPressMask);
}

void clear_keybinds(Config* cfg)
{
    XUngrabKey(display, cfg->key_next, cfg->key_mod, root_window);
    XUngrabKey(display, cfg->key_prev, cfg->key_mod, root_window);
    XUngrabKey(display, cfg->key_pause, cfg->key_mod, root_window);
    
    key_next = 0;
    key_prev = 0;
    key_pause = 0;
}

void destroy_tray_icons(void)
{
    for (int i = 0; i < 3; i++)
        XDestroyWindow(display, tray_windows[i]);

    for (int i = 0; i < 3; i++)
         XFreeGC(display, gc[i]);
    
    XCloseDisplay(display);

    free_rect(&next_rect);
    free_rect(&back_rect);
    free_rect(&pause_rect1);
    free_rect(&pause_rect1);
}

/* Keyconf */
void init_keyconf(void)
{
    XInitThreads();
    if((display = XOpenDisplay(NULL)) == NULL) {
        die("Cannot open display", 1);
    }

    root_window = DefaultRootWindow(display);

    XSelectInput(display, root_window, KeyPressMask);
}

void close_keyconf(void)
{
    XCloseDisplay(display);
}

void do_keyconf(void)
{
    XNextEvent(display, &event);

    switch(event.type)
    {
        case KeyPress:
            printf("KEY_CODE: %i\n", event.xkey.keycode);
            printf("MOD: %i\n", event.xkey.state);
        default:
            break;
    }
}