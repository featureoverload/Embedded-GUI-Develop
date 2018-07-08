# Makefile
# 
###
###
###

CC = gcc

object = CGIDebugLogc.o

.PYTHON: all clean

$(object): CGIDebugLogc.c CGIDebugLogc.h
	$(CC) -c $< -o $@

all: libCGIDebugLogc.a
	@echo "$'\033[0;46m==== build >> libCGIDebugLogc.a finish! ====\033[0m\n"

AR = ar
ARFLAGS = rvu
libCGIDebugLogc.a: libCGIDebugLogc.a(CGIDebugLogc.o)

libCGIDebugLogc.a(CGIDebugLogc.o): CGIDebugLogc.o
	$(AR) $(ARFLAGS) $@ $?



## use default rule
tester: libCGIDebugLogc.a 

tester.o: Examples/main.c
	$(CC) -c -I include $< -o $@


clean:
	rm -f *.o
	rm -f ./Examples/*.o
	rm -f libCGIDebugLogc.a

clean_tester:
	rm -f tester
	rm -f tester.o
