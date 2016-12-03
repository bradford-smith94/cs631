# Bradford Smith (bsmith8)
# Makefile for CS 631 HW 4
# 12/03/2016
################################################################################

PRG=aed
AUTHOR=bsmith8
PROJECT=hw4
SRC=aed.c usage.c
OBJ=$(SRC:.c=.o)
CC=gcc
RM=rm -f
LDLIBS+= -lbsd
LDFLAGS+=
CFLAGS+= -I./include
CPPFLAGS+= -pedantic-errors -Wall
DBFLGS= -g -DDEBUG
# implicit compiling rules:
# `$(CC) -c $(CPPFLAGS) $(CFLAGS)`

.PHONY: debug clean fclean re tar
all: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(PRG)

debug: fclean
debug: CPPFLAGS+= $(DBFLGS)
debug: all

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(PRG)

re: fclean all

tar: fclean
	tar -cvf $(AUTHOR)-$(PROJECT).tar --transform 's/^\./$(AUTHOR)/' ./*

