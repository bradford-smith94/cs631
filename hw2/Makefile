# Bradford Smith (bsmith8)
# Makefile for CS 631 HW 2
# 09/14/2016
################################################################################

PRG=
AUTHOR=bsmith8
PROJECT=hw2
SRC=
OBJ=$(SRC:.c=.o)
CC=gcc
RM=rm -f
INCLPATH=
LIBPATH=
LDLIBS=
LDFLAGS+=
# implicit compiling rules:
# C complied with: `$(CC) -c $(CPPFLAGS) $(CFLAGS)`
CFLAGS+=
CPPFLAGS+= -pedantic-errors -Wall
CXXFLAGS+=
DBFLGS= -g -DDEBUG

all: $(OBJ)
	#$(CXX) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(PRG)
	#$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(PRG)

debug: fclean
debug: CPPFLAGS+= $(DBFLGS)
debug: all

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(PRG)

re: fclean all

tar: fclean
	tar -cvf $(AUTHOR)-$(PROJECT).tar ./*

