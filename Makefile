CC=gcc -Wall -W -Werror -std=c99
CFLAGS=-O2 -g
CPPFLAGS:=$(shell pkg-config --cflags gtk+-2.0)
LIBS:=$(shell pkg-config --libs gtk+-2.0)
OBJECT=mand.o fatal.o

mand: $(OBJECT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECT) $(LIBS)

$(OBJECT): mand.h

clean:
	rm -f mand
	rm -f *.o
