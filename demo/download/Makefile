CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib

CGIC_SOURCE := cgic207

CPPFLAGS = -I $(CGIC_SOURCE)

LIBS=-L./ -lcgic


.PHONY: all install clean $(CGIC_SOURCE)


all: libcgic.a download.cgi

libcgic.a: $(CGIC_SOURCE)
	make --directory=$<
	cp $</$@ ./$@

utils.o: utils.c utils.h

download.cgi: download.c utils.o utils.h libcgic.a
	gcc utils.o $< $(CPPFLAGS) -o $@ ${LIBS}

clean:
	make --directory=$(CGIC_SOURCE) clean
	rm -f libcgic.a download.cgi utils.o
	@echo "\n======== Note ========"
	@echo "if you had been '$$ sudo make install'"
	@echo "you may want to run:"
	@echo "\t$$ sudo rm -f /var/www/cgi-bin/download.cgi"
	@echo "\t$$ sudo rm -f /var/www/cgi-bin/cgi_download.html"

install:
	cp download.cgi /var/www/cgi-bin/download.cgi
	cp cgi_download.html /var/www/cgi-bin/cgi_download.html
