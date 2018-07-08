#
# filename : Makefile
# author   : Joseph Lin
# E-mail   : joseph.lin@aliyun.com
# 
# reference: 《GNU make 项目管理》 - CH6
#
### 
### 2018/Jul/08 add CGIDebugLogc lib
### 

###
###  program tree:
### DebugLog_solution/
###   |-- Makefile
###   |-- LINUX-C_daemon/
###   |       |--- Makefile 
###   |        `-- ## source code to build libbecome_daemon.a
###   |-- lib/
###   |    |-- become_daemon/
###   |    |           |-- Makefile ## 调用 LINUX-C_daemon/ 下的Makefile
###   |    |            `- ## 将会copy *.a 到这里
###   |    |-- CGIUtils/ 
###   |    |      |-- Makefile
###   |     `-- echo2tmnl/
###   |           |-- Makefile
###   |
###   |-- test_src/  ## 可以 build 运行成功， 但是在本应用环境下，这个位置的program 没有用。 
###   |        |-- Makefile  ## build test_become_daemon* 出来！
###   |        `-- test_become_daemon.c
###   |
###   |-- src/                   ## <== $(CGIDebugLog)
###   |    |-- Makefile
###   |    |
###   |    |-- CGIDebugLogd/
###   |    |     |-- daemonEcho2.c
###   |    |     |-- CGIDebugLogd.py
###   |    |      `- Makefile
###   |    |-- socketClient.py
###   |    |
###   |     `- CGIDebugLogc/
###   |          |-- CGIDebugLogc.c
###   |          |-- CGIDebugLogc.h
###   |          |-- Makefile           ## build out libCGIDebugLogc.a
###   |           `- Examples/main.c
###   | 
###    `- others.
###
###    /makefile 将分别调用 lib/become_daemon/Makefile 和 src/Makefile
###


###
###  ":=" 这个符号的含义！？？？ 
###
lib_become_daemon := lib/become_daemon
lib_CGIUtils      := lib/CGIUtils
lib_echo2tmnl     := lib/echo2tmnl 

libraries         := $(lib_become_daemon) $(lib_CGIUtils) $(lib_echo2tmnl)

CGIDebugLog:= src

.PHONY: all $(CGIDebugLog) $(libraries) install clean


all: $(CGIDebugLog)
	cp $(CGIDebugLog)/daemonEcho2 $(CGIDebugLog)/CGIDebugLogd.py ./bin/
	cp $(CGIDebugLog)/libCGIDebugLogc.a ./bin/
	@echo "=======  DebugLog_solution/ build finish  ===========\n"



###
### 应用程序 与 库链接 的依存关系
###
$(CGIDebugLog): $(libraries)
	$(MAKE) --directory=$@
	@echo "----  $(CGIDebugLog) build finish  -----\n"

$(libraries):
	$(MAKE) --directory=$@
	@echo "----  $(libraries) build finish  -----\n"
###
### ^ 以上，调用了 test_basic_daemon/Makefile
###    和   调用了 lib/become_daemon/Makefile
###


install:
	$(MAKE) --directory=$(lib_CGIUtils)  install
	$(MAKE) --directory=$(lib_echo2tmnl) install

##### $ make clean 将会是一个问题 #####
clean:  
	$(MAKE) --directory=$(CGIDebugLog) clean && echo "\n"
#	$(MAKE) --directory=$(libraries) clean
	$(MAKE) --directory=$(lib_become_daemon) clean && echo "\n"
	$(MAKE) --directory=$(lib_CGIUtils) clean && echo "\n"
	$(MAKE) --directory=$(lib_echo2tmnl) clean && echo "\n"
	rm -f ./bin/*

## help - The default goal
MAKE = make
AWK  = awk
SORT = sort
PR   = pr
help:
	@ $(MAKE) --print-data-base --question |                \
	  $(AWK)  '/^[^.%][-A-Za-z0-9_]*:/                      \
	          { print substr($$1, 1, length($$1)-1) }'  |   \
	  $(SORT)  |                                            \
	  $(PR)  --omit-pagination --width=80 --columns=4


