include config.mk

VERSION = 3.02
CC      = /usr/bin/gcc

SRC = src/mpdq.c src/mpdt.c src/util.c src/cfg.c
OBJ = ${SRC:.c=.o}

mpdq: ${OBJ}
	@echo gcc -o mpdq ${OBJ} -lmpdclient ${LDFLAGS}
	@$(CC) -o mpdq ${OBJ} -lmpdclient ${LDFLAGS}
