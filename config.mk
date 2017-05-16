VERSION = 6.26

PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

MPDINC = /usr/include/mpd/
MPDLIB = `pkg-config --libs libmpdclient`

XINERAMALIBS = -L${X11LIB}
XINERAMAFLAGS = -DXINERAMA

INCS = -I. -I/usr/include -I${X11INC} -I${MPDINC}
LIBS = -L/usr/lib -lc -lrt -L${X11LIB} -lX11 ${XINERAMALIBS} ${MPDLIB}

CPPFLAGS = -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS} -D_FORTIFY_SOURCE=2 -DDEBUG
CFLAGS = -std=gnu99 -Wall -march=native -pipe -fstack-protector-strong --param=ssp-buffer-size=4 -O2 ${INCS} ${CPPFLAGS}
LDFLAGS = -s ${LIBS}

CC = gcc
