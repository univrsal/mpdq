/**
 * MPDQ
 * Music Player Daemon Query
 * Adding trayicons to control mpd
 * Licenced under MPL 2.0
 */
#include <cfg.h>

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

    // Initialize default values, incase loading the config goes wrong
    config->icon_scale = 1.0;
    config->root_window_song = 1;
    config->delay = 500;
    config->icon_color = 0xbbbbbb;
    config->max_song_length = 35;
    config->song_to_text_file = 0;
    config->file_path = "./mpd-nowplaying.txt";
    config->volume_timeout = 1500;

	if(access(path, F_OK) != -1)
	{
    	_log(append("Reading config from ", path));
    	_log("\n");
    	
    	FILE* cfg = open_file_rw(path);
    	
    	if (cfg == NULL) {
    		_log("Error opening config! Using default values\n");
    		return NULL;
    	}

    	char* c = read_line(cfg);
    	char* split;

    	while(c != NULL)
    	{
    		while(startsWith(c, "#"))
    			c = read_line(cfg);

    		split = strtok (c, "=");

    		if (strcmp(split, "song_to_root_window") == 0)
    		{
    			split = strtok(NULL, "=");
    			config->root_window_song = atoi(split);
    			_log(append("song_to_root_window=", split));
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
            else if (strcmp(split, "file_path") == 0)
            {
                split = strtok(NULL, "=");
                config->volume_timeout = atoi(split);
                _log(append("volume_timeout=", split));     
            }
    		c = read_line(cfg);
    	}
	}
	else
	{
        _log(append("No config creating it under ", path));
	    _log("\n");

	    FILE* cfg = fopen(path, "ab+");

	    fprintf(cfg, "# MPDQ v%s Config\n", version);
	    fprintf(cfg, "# The scaling of the tray icons in x/100 (50 Percent = 0.5) Default %f\n", config->icon_scale);
	    fprintf(cfg, "icon_scale=%f\n", config->icon_scale);
	 	fprintf(cfg, "# Wether to set the title of the root window to the current song (useful for dwm) Default %i\n", config->root_window_song);
	    fprintf(cfg, "song_to_root_window=%i\n", config->root_window_song);
        fprintf(cfg, "# The amount of milliseconds to wait between updates Default %i\n", config->delay);
        fprintf(cfg, "delay=%i\n", config->delay);
        fprintf(cfg, "# The color of the tray icons in RGB Hex format (RRGGBB) e.g. black = 000000, white = FFFFFF. Default bbbbbb\n");
        fprintf(cfg, "icon_color=bbbbbb\n");
        fprintf(cfg, "# The maximum character length of songs until they get cut off. Default %i\n", config->max_song_length);
        fprintf(cfg, "song_max_length=%i\n", config->max_song_length);
        fprintf(cfg, "# Wether to write the current song to a text file. Default %i\n", config->song_to_text_file);
        fprintf(cfg, "song_to_text_file=%i\n", config->song_to_text_file);
        fprintf(cfg, "# The path of the text file containing the current song. Default %s\n", config->file_path);
        fprintf(cfg, "file_path=%s\n", config->file_path);
        fprintf(cfg, "# The time in ms the volume will be shown when scrolling over the tray icons Default %i\n", config->volume_timeout);
        fprintf(cfg, "volume_timeout=%i\n", config->volume_timeout);
	}

	return config;
}