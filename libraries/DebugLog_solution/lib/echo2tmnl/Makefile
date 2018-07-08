###
### filename: Makefile
### function: 本Makefile 安装 CGIDebugLogd.py 等 需要的库
###

.PHONY: all clean install

sdist: setup.py echo2tmnl.py
	python3 setup.py sdist
	touch sdist
	@echo "$'\033[0;46m-----  echo2tmnl lib sdist finish!  -----\033[0m\n"
	
all:
	python3 setup.py sdist
	@echo "$'\033[0;46m-----  echo2tmnl lib sdist finish!  -----\033[0m\n"


	
install:
	python3 setup.py install

clean:
	rm -f sdist MANIFEST
	rm -rf dist

