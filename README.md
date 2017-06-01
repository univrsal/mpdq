# mpdq
MusicPlayerDaemon Query

![mpdq](http://i.imgur.com/QSDHzoO.jpg)
Adding traybar icons to control mpd. To make this work with dwm install the [sytray patch](http://dwm.suckless.org/patches/systray).
Tested with i3, dwm and Cinnamon!

Use the buttons to:
* Move to the previous song
* Move to the next song
* Play/Pause mpd
* Scroll up/down to increase/decrease the volume

There are configurable hotkeys for these actions

You can either build it yourself or get the [binaries](https://github.com/univrsal/mpdq/releases)

Building
```
$ git clone https://github.com/univrsal/mpdq.git
$ cd ./mpdq
$ make
```
You might need to adjust the Makefile and config.mk to make sure that the compiler finds xlib and libmpdclient 

Usage
```
chmod a+x ./mpdq
./mpdq
```
Requires mpd to be running and a playlist to be present.

Currently the config file allows you to change the following things
* Scaling of the tray icons
* Color of the tray icons
* Whether to display the current song in the dwm title bar (Title of the root window)
* Changing the update interval
* Whether to write the current song to a given file
* Change hotkeys
* Reverse traybar order for i3
