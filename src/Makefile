# 
# src/Makefile 
#
### 
###
###

Lib := ../lib
Head := ../include

CGIBIN := ../WorkPath/var/www/cgi-bin

CC = gcc
FLAGS = -I $(Head)

MAKE = make

MOVE = mv
MOVE_FLAGS =  

REMOVE = rm
REMOVE_FLAGS = -f

.PHONY: all clean clean_all install

all:
	$(CC) $(FLAGS) assistance.c -L $(Lib) -lcgic -lCGIDebugLogc -lflate -o assistance.cgi


install:
	$(MOVE) $(MOVE_FLAGS) assistance.cgi $(CGIBIN)

clean:
	$(REMOVE) $(REMOVE_FLAGS) *.cgi
