# Bradford Smith (bsmith8)
# Makefile for CS 631 sish
# 12/16/2016
################################################################################

PRG=sish
AUTHOR=bsmith8
PROJECT=sish
SRC=main.c executeCommand.c executePipeline.c freePipeline.c handleSigInt.c \
	init.c redirectIO.c removeIndex.c restoreIO.c tokenizePipeline.c
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

