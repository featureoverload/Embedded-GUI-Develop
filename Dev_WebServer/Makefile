# mini_httpd_dev/Makefile
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


PACKAGE_DIR = packages

WEB_SERVER = mini_httpd
WEB_SERV_SRC_DIR = mini_httpd

WEB_SERV_PACKAGE_NAME = mini_httpd-1.19.tar.gz
WEB_SERV_PACKAGE_DIR = mini_httpd-1.19


.PHONY: decompression run_patch init all clean install test runserver $(WEB_SERVER)

all: $(WEB_SERVER)


$(WEB_SERVER):
	$(MAKE) --directory=$(WEB_SERV_SRC_DIR)


decompression:
	$(TAR) $(TARFLAGS_GZ) $(PACKAGE_DIR)/$(WEB_SERV_PACKAGE_NAME) \
	-C $(shell pwd)
	$(MOVE) $(WEB_SERV_PACKAGE_DIR) $(WEB_SERV_SRC_DIR)

run_patch:
	chmod +w $(shell pwd)/$(WEB_SERV_SRC_DIR)/htpasswd.c
	PROJ_DIR=$(shell pwd) MINI_HTTPD_SOURCE_DIR=$(WEB_SERV_SRC_DIR) python3 \
		script/build_patch.py

init: decompression run_patch
	mkdir run
	mkdir -p run/var/log
	mkdir -p run/var/www/html
	mkdir -p run/var/www/cgi-bin


test:
	@echo "not implement yet"


runserver: $(WEB_SERVER)/$(WEB_SERVER)
	PROJ_DIR=$(shell pwd) CONF_DIR=conf TEMPLATE_FILE=mini_httpd.conf.jinja2 python3 \
		$(shell pwd)/script/mini_httpd_conf_patch.py

	$(WEB_SERV_SRC_DIR)/$(WEB_SERVER) -C $(shell pwd)/conf/mini_httpd.conf

stopserver:
	kill -15 $(shell cat $(shell pwd)/run/mini_httpd.pid)
	$(RM) $(shell pwd)/run/mini_httpd.pid


install:
	@echo "not implement yet"
