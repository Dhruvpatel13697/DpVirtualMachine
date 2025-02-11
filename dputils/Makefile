flags=-O3 -Wall -std=c2x -I.
ldflags=-fPIC -shared -ldl -D_GNU_SOURCE
libdir=/lib/x86_64-linux-gnu/
incdir=/usr/include

all: clean dputils.so

install: dputils.so
	cp dputils.so $(libdir)/libbu.so
	cp dputils.h  $(incdir)
	ldconfig

dputils.so: dputils.o
	cc ${flags} $^ -o $@ ${ldflags}

dputils.o: dputils.c
	cc ${flags} -c $^ $(ldflags)

clean:
	rm -f dputils.o dputils.so
