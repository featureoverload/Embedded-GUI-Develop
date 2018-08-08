CC	= gcc
CFLAGS	= -Wall
AR	= ar -r
RM	= rm -f

all:	libflate checktpl

libflate: flate.o
	$(AR) libflate.a flate.o

flate.o: flate.c
	$(CC) $(CFLAGS) flate.c -c -I.

clean:
	$(RM) flate.o libflate.a checktpl
