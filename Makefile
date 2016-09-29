include config.mk

VERSION = 3.02
CC      = /usr/bin/gcc

SRC = mpdq.c mpdt.c util.c cfg.c
OBJ = ${SRC:.c=.o}

mpdq: ${OBJ}
	@echo gcc -o mpdq ${OBJ} -lmpdclient ${LDFLAGS}
	@$(CC) -o mpdq ${OBJ} -lmpdclient ${LDFLAGS}
