# Bradford Smith (bsmith8)
# Makefile for CS 631 Midterm
# 09/20/2016
################################################################################

AUTHOR=bsmith8
PROJECT=midterm
CC=gcc
RM=rm -f
LDLIBS=
LDFLAGS+=
CFLAGS+= -I./include
CPPFLAGS+= -Wall -pedantic-errors
DBFLGS= -g -DDEBUG
# implicit compiling rules:
# `$(CC) -c $(CPPFLAGS) $(CFLAGS)`

all: ls

ls: ls.o

debug: fclean
debug: CPPFLAGS+= $(DBFLGS)
debug: all

clean:
	$(RM) *.o

#full clean
fclean: clean
	$(RM) a.out
	$(RM) ls

#re make
re: fclean all

tar: fclean
	tar -cvf $(AUTHOR)-$(PROJECT).tar --transform 's/^\./$(AUTHOR)/' --exclude 'ls-test' ./*

