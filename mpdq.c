/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include <mpd/client.h>
#include <mpd/stats.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <util.h>
#include <cfg.h>

/*　Vars */
static uint32_t     tray_width = 32, tray_height = 32;
static volatile int runFlag = 1;
static struct       mpd_connection   *conn;
static struct       mpd_status       *status;
static struct       mpd_song         *song;
static enum         mpd_state        state = 0;
static char         *song_name;
static char         *artist_name;
static char         *cmd;
static char         *base_cmd = "xsetroot -name \"";
static char         *cmd_end  = "\"";
static char         *space    = " - ";
static bool         no_refresh = 0;
static Config       *cfg;

/* Util Methods*/
void stop(int d) { runFlag = 0; }
void clean_up(void);

int main(int argc, char const *argv[])
{
    initX();
    cfg = create_or_open_cfg("./mpdq.cfg");

    // Vars
    int event_result = -1;
    debug = argc > 1;

    // Listen for CTRL + C
    signal(SIGINT, stop);

    // Connect to local MPD 
    conn = mpd_connection_new(NULL, 0, 0);
    if (conn)
        _log("Connected!\n", 0);
    else if (!conn) 
        _log("Connection failure!\n", 1);

    _log("heyo", 0);
    
    // Get song
    while (runFlag)
    {
        //draw_tray(state);
        status = mpd_run_status(conn);
        song   = mpd_run_current_song(conn);
        state  = mpd_status_get_state(status);
        
        if (!song) _log("Couldn't recieve current song!\n", 1);
        if (!status) _log("Couldn't recieve status of MPD!\n", 1);  

        // Get song info
        song_name = (char *) mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
        artist_name = (char *) mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);

        if (cfg->root_window_song)
            switch (state)
            {
                case 0:
                case 1:
                        system("xsetroot -name dwm-6.1"); // Stopped/Unknown status
                        no_refresh = 1;
                    break;
                case 2:
                        cmd = append(base_cmd, artist_name); // Playing
                        cmd = append(cmd, space);
                        cmd = append(cmd, song_name);
                        cmd = append(cmd, cmd_end);
                        no_refresh = 0;
            }

        event_result = handle_events();

        if (event_result > -1)
            tray_window_event(event_result, state, conn);

        draw_tray(state);

        if (cfg->root_window_song && !no_refresh && cmd != NULL) // No need to set the same name twice
            system(cmd);
        usleep(1000 * 500); // 500ms update cycle
    }

    printf("Disconnecting...\n");
    system("xsetroot -name dwm-6.1");

    // Close Connection & free resources
    clean_up();
    return 0;
}

void clean_up(void)
{
    destroy_tray_icons();
    mpd_status_free(status);
    mpd_connection_free(conn);
    mpd_song_free(song);
    song_name = NULL;
    artist_name = NULL;
    space = NULL;
    cmd = NULL;
    base_cmd = NULL;
    cmd_end = NULL;
}