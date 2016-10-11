# Bradford Smith (bsmith8)
# Makefile for CS 631 Midterm
# 10/11/2016
################################################################################

AUTHOR=bsmith8
PROJECT=midterm
SRC=ls.c cmp.c ftscmp.c get_winsize.c init.c print.c recursive_traverse.c \
	sanitize.c sort.c traverse.c
OBJ=$(SRC:.c=.o)
CC=gcc
RM=rm -f
LDLIBS+= -lbsd
LDFLAGS+=
CFLAGS+= -I./include
CPPFLAGS+= -Wall -pedantic-errors
DBFLGS= -g -DDEBUG
# implicit compiling rules:
# `$(CC) -c $(CPPFLAGS) $(CFLAGS)`

all: ls

ls: $(OBJ)

debug: fclean
debug: CPPFLAGS+= $(DBFLGS)
debug: all

clean:
	$(RM) *.o
	$(RM) *core*

#full clean
fclean: clean
	$(RM) a.out
	$(RM) ls

#re make
re: fclean all

tar: fclean
	tar -cvf $(AUTHOR)-$(PROJECT).tar --transform 's/^\./$(AUTHOR)/' --exclude 'ls-test' ./*

