
###
### filename: Makefile
###
### function: 本 Makefile 将 test_become_daemon.c 编译出可执行程序 
###

CC=gcc
RM=rm -f
#VPATH=include src lib bin
VPATH = ../../include

EXE=daemonEcho2

MACRO_FLAGS = 
#MACRO_FLAGS += -DSTRICT_DAEMON

CPPFLAGS = -I ../../include
CPPFLAGS += $(MACRO_FLAGS)


.PHONY: all clean
#all: test_become_daemon
#test_become_daemon: ../lib/become_daemon/libbecome_daemon.a

all: $(EXE)
$(EXE): ../../lib/become_daemon/libbecome_daemon.a
	$(CC) $(CPPFLAGS) daemonEcho2.c $^ -o daemonEcho2
	@echo "$'\033[0;46m------ $(EXE) build finish!  ------\033[0m\n"

clean:
	rm -f $(EXE)
