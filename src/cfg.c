/**
* MPDQ
* Music Player Daemon Query
* Adding trayicons to control mpd
* Licenced under MPL 2.0
*/
#include <src/cfg.h>

FILE* open_file_rw(const char* path)
{
    FILE * f = fopen(path, "r");
    if (f == NULL)
        exit(EXIT_FAILURE);
    return f;
}

void close_file(FILE* file)
{
    fclose(file);
}

char* read_line(FILE* file)
{
    char* line;
    size_t len = 0;
    ssize_t read;
    read = getline(&line, &len, file);
    if (read == -1)
    {
        return NULL;
    }

    return line;
}

void write_line(FILE* file, char* c)
{
    fprintf(file, c);
}

Config* create_or_open_cfg(char* path)
{
    Config* config = malloc(sizeof(Config));
    Display*    dpy     = XOpenDisplay(0);

    // Initialize default values, incase loading the config goes wrong
    config->song_to_root_window = true;
    config->reverse_icons       = false;
    config->song_to_text_file   = false;
    config->enable_hotkeys      = true;
    config->enable_icons        = true;

    config->icon_scale          = 1.0;
    config->delay               = 500;
    config->icon_color          = 0xbbbbbb;
    config->max_song_length     = 35;
    config->file_path           = "./mpd-nowplaying.txt";
    config->title_text          = "dwm-6.1";
    config->volume_timeout      = 1500;
    config->xOffset             = 0;
    config->yOffset             = 0;

    config->key_next            = XKeysymToKeycode(dpy, XK_N);
    config->key_prev            = XKeysymToKeycode(dpy, XK_B);
    config->key_pause           = XKeysymToKeycode(dpy, XK_P);
    config->key_mod             = 128;

    dpy = NULL;

    if(access(path, F_OK) != -1)
    {
        _log(append("Reading config from ", path));
        _log("\n");

        FILE* cfg = open_file_rw(path);

        if (cfg == NULL)
        {
            _log("Error opening config! Using default values\n");
            return NULL;
        }

        char* c = read_line(cfg);
        char* split;

        while(c != NULL)
        {
            while(starts_with(c, "#"))
                c = read_line(cfg);

            split = strtok (c, "=");

            if (strcmp(split, "song_to_root_window") == 0)
            {
                split = strtok(NULL, "=");
                config->song_to_root_window = atoi(split);
                _log(append("song_to_root_window=", split));
            }
            else if (strcmp(split, "enable_icons") == 0)
            {
                split = strtok(NULL, "=");
                config->enable_icons = atoi(split);
                _log(append("enable_icons=", split));
            }
            else if (strcmp(split, "enable_hotkeys") == 0)
            {
                split = strtok(NULL, "=");
                config->enable_hotkeys = atoi(split);
                _log(append("enable_hotkeys=", split));
            }
            else if (strcmp(split, "icon_scale") == 0)
            {
                split = strtok(NULL, "=");
                config->icon_scale = atof(split);
                _log(append("icon_scale=", split));
            }
            else if(strcmp(split, "delay") == 0)
            {
                split = strtok(NULL, "=");
                config->delay = atoi(split);
                _log(append("delay=", split));
            }
            else if (strcmp(split, "icon_color") == 0)
            {
                split = strtok(NULL, "=");
                config->icon_color = (int) strtol(split, NULL, 16);
                _log(append("icon_color=", split));
            }
            else if (strcmp(split, "song_max_length") == 0)
            {
                split = strtok(NULL, "=");
                config->max_song_length = atoi(split);
                _log(append("song_max_length=", split));
            }
            else if (strcmp(split, "song_to_text_file") == 0)
            {
                split = strtok(NULL, "=");
                config->song_to_text_file = atoi(split);
                _log(append("song_to_text_file=", split));
            }
            else if (strcmp(split, "file_path") == 0)
            {
                split = strtok(NULL, "=");
                config->file_path = split;
                _log(append("file_path=", split));
            }
            else if (strcmp(split, "volume_timeout") == 0)
            {
                split = strtok(NULL, "=");
                config->volume_timeout = atoi(split);
                _log(append("volume_timeout=", split));
            }
            else if (strcmp(split, "icon_reverse_icons") == 0)
            {
                split = strtok(NULL, "=");
                config->reverse_icons = atoi(split);
                _log(append("icon_reverse_icons=", split));
            }
            else if (strcmp(split, "key_next") == 0)
            {
                split = strtok(NULL, "=");
                config->key_next = atoi(split);
                _log(append("key_next=", split));
            }
            else if (strcmp(split, "key_prev") == 0)
            {
                split = strtok(NULL, "=");
                config->key_prev = atoi(split);
                _log(append("key_prev=", split));
            }
            else if (strcmp(split, "key_pause") == 0)
            {
                split = strtok(NULL, "=");
                config->key_pause = atoi(split);
                _log(append("key_pause=", split));
            }
            else if (strcmp(split, "key_mod") == 0)
            {
                split = strtok(NULL, "=");
                config->key_mod = atoi(split);
                _log(append("key_mod=", split));
            }
            else if (strcmp(split, "title_text") == 0)
            {
                split = strtok(NULL, "=");
                config->title_text = split;
                _log(append("title_text=", split));
            }
            else if (strcmp(split, "x_offset") == 0)
            {
                split = strtok(NULL, "=");
                config->xOffset = atoi(split);
                _log(append("x_offset=", split));
            }
            else if (strcmp(split, "y_offset") == 0)
            {
                split = strtok(NULL, "=");
                config->yOffset = atoi(split);
                _log(append("y_offset=", split));
            }

            c = read_line(cfg);
        }
    }
    else
    {
        _log(append("No config creating it under ", path));
        _log("\n");

        FILE* cfg = fopen(path, "ab+");

        fprintf(cfg, "# MPDQ %s Config\n", version);
        
        fprintf(cfg, "# Set to 0 to disable hotkeys Default %i\n", config->enable_hotkeys);
        fprintf(cfg, "enable_hotkeys=%i\n", config->enable_hotkeys);

        fprintf(cfg, "# Set to 0 to disable the tray icons Default %i\n", config->enable_icons);
        fprintf(cfg, "enable_icons=%i\n", config->enable_icons);

        fprintf(cfg, "# Whether to set the title of the root window to the current song (useful for dwm) Default %i\n", config->song_to_root_window);
        fprintf(cfg, "song_to_root_window=%i\n", config->song_to_root_window);
        
        fprintf(cfg, "# When set to 1 the icons will be reverse_iconsd Default %i\n", config->reverse_icons);
        fprintf(cfg, "icon_reverse_icons=%i\n", config->reverse_icons);
        
        fprintf(cfg, "# Whether to write the current song to a text file. Default %i\n", config->song_to_text_file);
        fprintf(cfg, "song_to_text_file=%i\n", config->song_to_text_file);

        fprintf(cfg, "# The scaling of the tray icons in x/100 (50 Percent = 0.5) Default %f\n", config->icon_scale);
        fprintf(cfg, "icon_scale=%f\n", config->icon_scale);
        
        fprintf(cfg, "# The amount of milliseconds to wait between updates Default %i\n", config->delay);
        fprintf(cfg, "delay=%i\n", config->delay);
        
        fprintf(cfg, "# The color of the tray icons in RGB Hex format (RRGGBB) e.g. black = 000000, white = FFFFFF. Default bbbbbb\n");
        fprintf(cfg, "icon_color=bbbbbb\n");
        
        fprintf(cfg, "# The maximum character length of songs until they get cut off. Default %i\n", config->max_song_length);
        fprintf(cfg, "song_max_length=%i\n", config->max_song_length);
        
        fprintf(cfg, "# The path of the text file containing the current song. Default %s\n", config->file_path);
        fprintf(cfg, "file_path=%s\n", config->file_path);
        
        fprintf(cfg, "# The time in ms the volume will be shown when scrolling over the tray icons Default %i\n", config->volume_timeout);
        fprintf(cfg, "volume_timeout=%i\n", config->volume_timeout);
        
        fprintf(cfg, "#The text to set the dwm title bar to when no song is playing or the program exits. Default %s\n", config->title_text);
        fprintf(cfg, "title_text=%s\n", config->title_text);
        
        fprintf(cfg, "# Offsets. X and Y shift of the tray icons (Top left corner) Default X: %i, Y: %i\n", config->xOffset, config->yOffset);
        fprintf(cfg, "x_offset=%i\n", config->xOffset);
        fprintf(cfg, "y_offset=%i\n", config->yOffset);

        fprintf(cfg, "# The keybinds. key_next (Next song. Default %i), key_prev (Previous song. Default %i), key_pause (Default %i), key_mod (Modifier eg. Ctrl. Default %i)\n", config->key_next, config->key_prev, config->key_pause, config->key_mod);
        fprintf(cfg, "# To find out keycodes & modifier masks run `xev | grep state` and look for state and keycode. State has to be converted from hex to decimal\n");
        fprintf(cfg, "key_next=%i\n", config->key_next);
        fprintf(cfg, "key_prev=%i\n", config->key_prev);
        fprintf(cfg, "key_pause=%i\n", config->key_pause);
        fprintf(cfg, "key_mod=%i\n", config->key_mod);
    }

    return config;
}
