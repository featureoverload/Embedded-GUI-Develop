# Makefile
# 
#
###
### 
###

MAKE = make

CD = cd
MOVE = mv
COPY = cp
RM   = rm 

#
# /
# |---- libraries/
# |         |---- mini-httpd...tar.gz
# |         |---- DebugLog_solution.zip
# |
# |
# |---- Makefile(<--this file)
# |
# |---- src/
# |---- html/
# |---- res/
# |
# |
# |---- WorkPath/
#


###
### 这里设定 libraries 的相关内容！
### 这里是文件夹
###
Libraries      := libraries

Lib = lib
CGI_PROGRAM = src

Include = include

.PHONY: $(Libraries) $(CGI_PROGRAM) LIB all install clean BIN clean_all \
		$(Lib) $(Include)



###
### make all ==> 将所有 src/*.c --编译成--> src/*.cgi 
###   需要 web.so cgic.so 等的支持， 依赖于 `$ make lib`
###
#all: --mini_httpd-- --cgic.so-- --web.so--

## make all = make lib + make src ##+ make Lib&Bin
all: $(CGI_PROGRAM)

init: $(Libraries)/mini_httpd-1.19.tar.gz $(Libraries)/DebugLog_solution.zip
	cd $(Libraries) && tar -xzvf mini_httpd-1.19.tar.gz
	cd $(Libraries) && chmod +w mini_httpd-1.19/htpasswd.c
	cd $(Libraries)	&& python3 build_patch.py

## DebugLog_solution.zip out of date. there is DebugLog_solution/ on trunk.
#	cd $(Libraries) && unzip DebugLog_solution.zip
#	... else ...

###
### 应用程序 与 库链接 的依存关系
### (*.cgi)    (mini_httpd, cgiDebugLogd, cgic.so web.so)
###
$(CGI_PROGRAM): $(Libraries) LIB BIN 

###
### 以下编译了 *.cgi， 并且确保依赖库要被编译。
###
$(CGI_PROGRAM):
	$(MAKE) --directory=$@   
## 
## ^^ 以上， 调用了 src/Makefile & libraries/Makefile
##


###
### make lib => 需要 mini_httpd;  cgic.a, flate.a, cgiDebugLog.a 
###
LIB: $(Libraries)
	$(MOVE) $(Libraries)/libCGIDebugLogc.a ./lib
	$(MAKE) --directory=lib all 	## for libcigc.a

$(Libraries):
	$(MAKE) --directory=$@
	@echo "======= finished build libraries/  =======\n"

BIN:
	$(MOVE) $(Libraries)/mini_httpd ./bin
	$(MOVE) $(Libraries)/CGIDebugLogd.py ./bin
###
### usage LINUX-C_daemon
###
#	$(COPY) $(Libraries)/daemonEcho2 ./bin
###
### usage python-dameon
###
	$(MOVE) $(Libraries)/DaemonEcho2 ./bin

Include:
	$(MAKE) --directory=include 

###
### make install ==> 将 src/*cgi html/*.html res/*.json *.js *.css 安装
###   需要检查当前是否有 CGIDebugLogd 在运行， 
###   以及 mini_httpd 是否在运行。
###   没有的话， 需要打印提示消息！
###
install:
	$(MAKE) --directory=$(Libraries) install



clean:
	$(MAKE) --directory=$(Lib) clean
	$(MAKE) --directory=$(CGI_PROGRAM) clean
	rm -f lib/*.a
	rm -f bin/*



clean_all:
	$(MAKE) --directory=$(Lib) clean_all
	$(MAKE) --directory=$(CGI_PROGRAM) clean
	rm -f bin/*
	
