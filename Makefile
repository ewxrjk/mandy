CC=gcc -Wall -W -Werror -std=c99
CFLAGS=-O2 -g
CPPFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LIBS=$(shell pkg-config --libs gtk+-2.0)

mand: mand.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f mand
	rm -f *.o
