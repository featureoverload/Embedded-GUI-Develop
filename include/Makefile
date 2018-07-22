# include/Makefile
#
#

CC = gcc

MAKE = make

COPY = cp

Libraries := ../libraries

Flate := ../lib/flate
CGIC := ../lib/cgic

.PHONY: all clean  $(Libraries) $(Flate) $(CGIC)

all: cgic.h flate.h CGIDebugLogc.h

# libCGIDebugLogc.a: $()
cgic.h:
	ln -s $(CGIC)/cgic.h ./cgic.h

flate.h:
	ln -s $(Flate)/flate.h ./flate.h

CGIDebugLogc.h:
	ln -s $(Libraries)/DebugLog_solution/src/CGIDebugLogc/$@ $@



# clean:



# clean_all:

