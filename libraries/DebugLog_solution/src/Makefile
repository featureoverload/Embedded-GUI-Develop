
###
### filename: Makefile
###
### function: 本 Makefile 将 test_become_daemon.c 编译出可执行程序 
###

CC=gcc
RM=rm -f
#VPATH=include src lib bin
VPATH = ../include

# EXE=daemonEcho2 CGIDebugLogd.py libCGIDebugLogc.a
EXE=DaemonEcho2 CGIDebugLogd.py libCGIDebugLogc.a


MACRO_FLAGS = 
#MACRO_FLAGS += -DSTRICT_DAEMON

CPPFLAGS = -I ../include
CPPFLAGS += $(MACRO_FLAGS)

CGIDebugLogd := CGIDebugLogd
CGIDebugLogc := CGIDebugLogc

.PHONY: all clean $(CGIDebugLogc) $(CGIDebugLogd)
#all: test_become_daemon
#test_become_daemon: ../lib/become_daemon/libbecome_daemon.a

all: $(EXE)

clean:
	rm -f $(EXE)
	$(MAKE) --directory=$(CGIDebugLogc) clean
	$(MAKE) --directory=$(CGIDebugLogd) clean

clean_all:
	rm -f $(EXE)
	$(MAKE) --directory=$(CGIDebugLogc) clean
	$(MAKE) --directory=$(CGIDebugLogd) clean

# daemonEcho2:
	# $(MAKE) --directory=$(CGIDebugLogd) getdaemonEcho2
DaemonEcho2:
	$(MAKE) --directory=$(CGIDebugLogd) getDaemonEcho2

CGIDebugLogd.py: $(CGIDebugLogd)/CGIDebugLogd.py
	cp $< ./


libCGIDebugLogc.a: $(CGIDebugLogc)/CGIDebugLogc.o
	ar rvu $@ $^
$(CGIDebugLogc):
	$(MAKE) --directory=$@
