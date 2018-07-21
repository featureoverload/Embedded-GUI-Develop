# libraries/Makefile
# Author: Joseph Lin
# E-mail: joseph.lin@aliyun.com
#
###
### ------ 
###   v0.0.1
### change log: build CGIDebugLogd 相关
###
### ------
###   v0.0.2
### change log: build mini_httpd 相关
### 
### ------
###   v0.0.3
### change log: build CGIDebugLogc 相关
###

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

EXE_CGIDebugLog := CGIDebugLogd.py daemonEcho2 libCGIDebugLogc.a
EXE_MINIHTTPD   := mini_httpd

Lib_mini_httpd := mini_httpd-1.19
Lib_CGIDebugLog := DebugLog_solution

LibCGIDebugLogc := libCGIDebugLogc.a

Lib_cgi := cgic
Lib_template := flate
## cgic, flate, cgi-debug => **.a/so web.a

#Libraries := $(Lib_mini_httpd) $(Lib_CGIDebugLog) $(Lib_cgi) $(Lib_template)

.PHONY: all  install clean $(Lib_CGIDebugLog) $(Lib_mini_httpd)

##                           cgic.so           web.so
all:  $(EXE_MINIHTTPD) CGIDebugLog 

$(EXE_MINIHTTPD): $(Lib_mini_httpd)
	$(COPY) $(Lib_mini_httpd)/mini_httpd ./
	@echo "$'\033[0;41m----- finish build mini_httpd! ---------\033[0m\n"

CGIDebugLog: $(EXE_CGIDebugLog)
	@echo "$'\033[0;41m----- finish build CGIDebugLog! ---------\033[0m\n"
$(EXE_CGIDebugLog): $(Lib_CGIDebugLog)

CGIDebugLogd.py:
	$(COPY) $(Lib_CGIDebugLog)/bin/$@ ./
daemonEcho2:
	$(COPY) $(Lib_CGIDebugLog)/bin/$@ ./
libCGIDebugLogc.a:
	$(COPY) $(Lib_CGIDebugLog)/bin/$@ ./


### 下面：
### 应用程序 与 库链接 的依存关系
### -------
### 这里，应用程序并非如此，应用程序在 make lib 的时候没有马上编译，
### make lib 编译出库，并且copy 到一定的位置，提供 应用程序编译和运行环境。
###


$(Lib_CGIDebugLog):
	$(MAKE) --directory=$(Lib_CGIDebugLog)
	@echo "\n"
	


$(Lib_mini_httpd):
	$(MAKE) --directory=mini_httpd-1.19
	@echo "\n"

	


install:
## for python lib 
	$(MAKE) --directory=$(Lib_CGIDebugLog) install 


clean:
	rm -f daemonEcho2 mini_httpd CGIDebugLogd.py libCGIDebugLogc.a

clean_all:
	rm -f daemonEcho2 mini_httpd CGIDebugLogd.py libCGIDebugLogc.a
	$(MAKE) --directory=mini_httpd-1.19 clean
	$(MAKE) --directory=DebugLog_solution clean
	@echo "$'\033[0;41m !!!!!!  not remove mini_httpd-1.19 directory  !!!!!!\033[0m\n"


