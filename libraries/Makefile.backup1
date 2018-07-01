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

UZIP = uzip

MAKE = make

CD = cd
MOVE = mv
COPY = cp
RM   = rm 

Lib_mini_httpd := mini_httpd
Lib_CGIDebugLogd := CGIDebugLog
Lib_cgi := cgic
Lib_template := flate
## cgic, flate, cgi-debug => **.a/so web.a

#Libraries := $(Lib_mini_httpd) $(Lib_CGIDebugLogd) $(Lib_cgi) $(Lib_template)

.PHONY: all   $(libraries) clean

##                           cgic.so           web.so
all: mini_httpd CGIDebugLogd cgic.a   flate.a  cgidbg.a



### 下面：
### 应用程序 与 库链接 的依存关系
### -------
### 这里，应用程序并非如此，应用程序在 make lib 的时候没有马上编译，
### make lib 编译出库，并且copy 到一定的位置，提供 应用程序编译和运行环境。
###

##
## 1. 解压到 mini_httpd 文件夹中
## 2. 修改编译不能通过的 htpasswd.c 文件
## 3. 编译 mini_httpd
##
mini_httpd: $(Lib_mini_httpd)
#### change getline() function on /mini_httpd-1.19/htpasswd.c
#	change code...
	$(MAKE) --directory=$^
	$(COPY) $^/$@ ./

CGIDebugLogd: $(Lib_CGIDebugLogd)
	$(MAKE) --directory=$^
	$(COPY) $^/$@ ./

cgic.a: $(Lib_cgi)
	$(MAKE) --directory=$^
	$(COPY) $^/$@ ./

flate.a: $(Lib_template)
	$(MAKE) --directory=$^
	$(COPY) $^/$@ ./

cgidbg.a: $(Lib_CGIDebugLogd)
#	???$(COPY) $^/$@ ./


$(Lib_mini_httpd): mini_httpd-1.19.tar.gz
	$(TAR) $(TARFLAGS_GZ) $^ mini_httpd

mini_httpd-1.19.tar.gz: 
	wget http://www.acme.com/software/mini_httpd/mini_httpd-1.19.tar.gz


$(Lib_CGIDebugLogd): CGIDebugLogd.zip
	$(UZIP) $^


$(Lib_cgi): cigc.zip
	$(UZIP) $^


$(Lib_template): flate.zip
	$(UZIP) $^

