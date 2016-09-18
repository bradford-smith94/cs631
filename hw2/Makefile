# Bradford Smith (bsmith8)
# Makefile for CS 631 HW 2
# 09/18/2016
################################################################################

AUTHOR=bsmith8
PROJECT=hw2
CC=gcc
RM=rm -f
IOSIZE=$(shell stat -c "%o" Makefile)
LDLIBS=
LDFLAGS+=
CFLAGS+= -DBUF_SIZE=$(IOSIZE)
CPPFLAGS+= -Wall -pedantic-errors
DBFLGS= -g -DDEBUG
# implicit compiling rules:
# `$(CC) -c $(CPPFLAGS) $(CFLAGS)`

all: tcp tcpm

tcp: tcp.o

tcpm: tcpm.o

debug: fclean
debug: CPPFLAGS+= $(DBFLGS)
debug: all

clean:
	$(RM) *.o

#full clean
fclean: clean
	$(RM) a.out
	$(RM) tcp
	$(RM) tcpm

#re make
re: fclean all

tar: fclean
	tar -cvf $(AUTHOR)-$(PROJECT).tar --transform 's/^\./bsmith8/' ./*

