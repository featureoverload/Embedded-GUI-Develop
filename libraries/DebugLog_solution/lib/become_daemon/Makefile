
###
### filename: Makefile
### 
### function: 本Makefile 调用 LINUX-C_daemon/ 目录下的 Makefile， 
###           并且将其编译好了的 lib/libbecome_daemon.a 拷贝到本目录下
###
###

BECOME_DAEMON_PATH = ../../LINUX-C_daemon
BECOME_DAEMON_PACKAGE = $(BECOME_DAEMON_PATH)/lib/libbecome_daemon.a

COPY       = cp
COPYFLAGES =  


libbecome_dameon := $(BECOME_DAEMON_PATH)

.PHONY: all $(libbecome_dameon) clean


libbecome_daemon.a: $(libbecome_dameon)
	$(COPY) $(COPYFLAGES) $(BECOME_DAEMON_PACKAGE) ./
	@echo "$'\033[0;46m----  finished build lib/libbecome_daemon.a  ----\033[0m\n"

$(libbecome_dameon):
	$(MAKE) --directory=$@ all


#all: libbecome_daemon.a $(libbecome_dameon)

clean:
	rm -f libbecome_daemon.a
	$(MAKE) --directory=$(libbecome_dameon) clean
