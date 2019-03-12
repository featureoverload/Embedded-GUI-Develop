CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib

CGIC_SOURCE := cgic207

CPPFLAGS = -I $(CGIC_SOURCE)

LIBS=-L./ -lcgic


.PHONY: all install clean $(CGIC_SOURCE)


all: libcgic.a download_a_file.cgi

libcgic.a: $(CGIC_SOURCE)
	make --directory=$<
	cp $</$@ ./$@

download_a_file.cgi: download_a_file.c libcgic.a
	gcc $< $(CPPFLAGS) -o $@ ${LIBS}

clean:
	make --directory=$(CGIC_SOURCE) clean
	rm -f libcgic.a download_a_file.cgi
	@echo "\n======== Note ========"
	@echo "if you had been '$$ sudo make install'"
	@echo "you may want to run:"
	@echo "\t$$ sudo rm -f /var/www/cgi-bin/download_a_file.cgi"
	@echo "\t$$ sudo rm -f /var/www/cgi-bin/cgi_download_a_file.html"

install:
	cp download_a_file.cgi /var/www/cgi-bin/download_a_file.cgi
	cp cgi_download_a_file.html /var/www/cgi-bin/cgi_download_a_file.html
