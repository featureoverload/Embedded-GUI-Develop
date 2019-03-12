CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib

CGIC_SOURCE := cgic207

CPPFLAGS = -I $(CGIC_SOURCE)

LIBS=-L./ -lcgic


.PHONY: all install clean $(CGIC_SOURCE)

all: libcgic.a hellowrold.cgi

libcgic.a: $(CGIC_SOURCE)
	make --directory=$<
	cp $</$@ ./$@

helloworld.cgi: helloworld.c libcgic.a
	gcc $< $(CPPFLAGS) -o $@ ${LIBS}

clean:
	make --directory=$(CGIC_SOURCE) clean
	rm -f libcgic.a helloworld.cgi

install:
	cp helloworld.cgi /var/www/cgi-bin/helloworld.cgi
