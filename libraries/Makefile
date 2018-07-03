# Makefile
# 
#
#

## get mini_httpd-<version>.tar.gz packages
## $ wget http://www.acme.com/software/mini_httpd/mini_httpd-1.19.tar.gz
##

TAR = tar
TARFLAGS_GZ = -xzvf
TARFLAGS_BZ2 = -jxvf

UNZIP = unzip

MAKE = make

CD = cd
MOVE = mv
COPY = cp
RM   = rm 

EXE_CGIDebugLog := CGIDebugLogd.py daemonEcho2


Lib_mini_httpd := mini_httpd-1.19
Lib_CGIDebugLog := DebugLog_solution
Lib_cgi := cgic
Lib_template := flate
## cgic, flate, cgi-debug => **.a/so web.a

#Libraries := $(Lib_mini_httpd) $(Lib_CGIDebugLog) $(Lib_cgi) $(Lib_template)

.PHONY: all  install $(Lib_CGIDebugLog) clean

##                           cgic.so           web.so
all:  $(Lib_CGIDebugLog) $(Lib_mini_httpd)



### 下面：
### 应用程序 与 库链接 的依存关系
### -------
### 这里，应用程序并非如此，应用程序在 make lib 的时候没有马上编译，
### make lib 编译出库，并且copy 到一定的位置，提供 应用程序编译和运行环境。
###


$(Lib_CGIDebugLog):  DebugLog_solution.zip
	$(UNZIP) DebugLog_solution.zip 
	$(MAKE) --directory=$(Lib_CGIDebugLog)
	$(COPY) $(Lib_CGIDebugLog)/src/CGIDebugLogd.py $(Lib_CGIDebugLog)/src/daemonEcho2 ./


$(Lib_mini_httpd): mini_httpd-1.19.tar.gz
	$(TAR) $(TARFLAGS_GZ) $^
	chmod +w mini_httpd-1.19/htpasswd.c
	./build_patch.py
	$(MAKE) --directory=mini_httpd-1.19
	$(COPY) $(Lib_mini_httpd)/mini_httpd ./


install:
## for python lib 
	$(MAKE) --directory=$(Lib_CGIDebugLog) install 

