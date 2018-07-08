
###
### filename : Makefile
### author   : Joseph Lin
### E-mail   : joseph.lin@aliyun.com
### 
### reference: 《GNU make 项目管理》 - CH6
###
### 
### 

###
### program tree:
###   /
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
###   |-- src/
###   |    |-- Makefile
###   |    |-- daemonEcho2.c
###   |    |-- CGIDebugLogd.py
###   |     `- socketClient.py
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

CGIDebugLogd:= src

.PHONY: all $(CGIDebugLogd) $(libraries) install clean
all: $(CGIDebugLogd)


###
### 应用程序 与 库链接 的依存关系
###
$(CGIDebugLogd): $(libraries)


$(CGIDebugLogd) $(libraries):
	$(MAKE) --directory=$@
###
### ^ 以上，调用了 test_basic_daemon/Makefile
###    和   调用了 lib/become_daemon/Makefile
###


install:
	$(MAKE) --directory=$(lib_CGIUtils)  install
	$(MAKE) --directory=$(lib_echo2tmnl) install

##### $ make clean 将会是一个问题 #####
clean:  
	$(MAKE) --directory=$(CGIDebugLogd) clean && echo "\n"
#	$(MAKE) --directory=$(libraries) clean
	$(MAKE) --directory=$(lib_become_daemon) clean && echo "\n"
	$(MAKE) --directory=$(lib_CGIUtils) clean && echo "\n"
	$(MAKE) --directory=$(lib_echo2tmnl) clean && echo "\n"

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


