CFLAGS  = `pkg-config --cflags gtk+-2.0`
LFLAGS   = `pkg-config --libs gtk+-2.0`

default: build

build: onkey.o
	gcc onkey.o -g -lX11 -lXtst -o onkey ${LFLAGS}

onkey.o:
	gcc ${CFLAGS} -c onkey.c

clean:
	rm -f onkey.o

install:
	chmod 775 onkey
	cp onkey /usr/local/bin

uninstall:
	rm -f /usr/local/bin/onkey
