
###
### filename: Makefile
###
### function: 本 Makefile 将 test_become_daemon.c 编译出可执行程序 
###

CC=gcc
RM=rm -f

COPY=cp
MOVE=mv

#VPATH=include src lib bin
VPATH = ../../include

# EXE=daemonEcho2
EXE=DaemonEcho2

MACRO_FLAGS = 
#MACRO_FLAGS += -DSTRICT_DAEMON

CPPFLAGS = -I ../../include
CPPFLAGS += $(MACRO_FLAGS)


.PHONY: all clean getdaemonEcho2
#all: test_become_daemon
#test_become_daemon: ../lib/become_daemon/libbecome_daemon.a

all: $(EXE)

# $(EXE): ../../lib/become_daemon/libbecome_daemon.a
# 	$(CC) $(CPPFLAGS) daemonEcho2.c $^ -o daemonEcho2
# 	@echo "$'\033[0;46m------ $(EXE) build finish!  ------\033[0m\n"

getdaemonEcho2: ../../lib/become_daemon/libbecome_daemon.a
	$(CC) $(CPPFLAGS) daemonEcho2.c $^ -o ../daemonEcho2
	@echo "$'\033[0;46m------ $(EXE) build finish!  ------\033[0m\n"

## src DebugLog_solution libraries Dev 
getDaemonEcho2: ../../../../WorkPath/lib/python3.5/daemon
	$(COPY) DaemonEcho2 ../
	@echo "$'\033[0;46m------ $(EXE) build finish!  ------\033[0m\n"

clean:
###
### usage LINUX-C_daemon
###
#	rm -f $(EXE)
###
### usage python-daemon
###
	@echo "<path>/libraries/DebugLog_solution/src/CGIDebugLogd/ clean!\n"
