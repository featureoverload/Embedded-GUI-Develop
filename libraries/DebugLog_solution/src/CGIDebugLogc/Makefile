# Makefile
# 
###
###
###

CC = gcc


.PYTHON: all clean

all: libCGIDebugLogc.a
	@echo "==== build >> libCGIDebugLogc.a finish! ====\n"

AR = ar
ARFLAGS = rv
libCGIDebugLogc.a: libCGIDebugLogc.a(CGIDebugLogc.o)

libCGIDebugLogc.a(CGIDebugLogc.o): CGIDebugLogc.o
	$(AR) $(ARFLAGS) $@ $?


CGIDebugLogc.o: CGIDebugLogc.c CGIDebugLogc.h
	$(CC) -c $< -o $@


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
