# lib/Makefile
#
#

CC = gcc

MAKE = make

COPY = cp

Libraries := ../libraries

Flate := flate
CGIC := cgic

.PHONY: all clean  clean_cgic clean_all $(Libraries) $(Flate) $(CGIC)

all: libCGIDebugLogc.a libcgic.a libflate.a

# libCGIDebugLogc.a: $()
libCGIDebugLogc.a:
	$(COPY) $(Libraries)/$@ ./
	@echo ">>>>>   finish build libCGIDebugLogc.a   <<<<<<"


libcgic.a: $(CGIC)
#    -[o] 根据 “配置文件”（未实现）决定 cgic 的版本； 默认-cgic, 201-cgic201
	make --directory=$<
	cp $</libcgic.a ./
	@echo ">>>>>   finish build libcgic.a   <<<<<<"

libflate.a: $(Flate)
	cp $</libflate.a ./
	@echo ">>>>>   finish build libcgic.a   <<<<<<"

$(Flate):
	make --directory=$@

clean:
	rm -f libcgic.a
	$(MAKE) --directory=cgic clean 		### -[o] directory depends on config file.

	rm -f libCGIDebugLogc.a
	$(MAKE) --directory=$(Libraries) clean 

	rm -f libflate.a
	$(MAKE) --directory=$(Flate) clean
	
	rm -f *.so


clean_cgic:
	rm -f libcgic.a
	$(MAKE) --directory=cgic clean 		### -[o] directory depends on config file.


clean_all:
	rm -f libcgic.a
	$(MAKE) --directory=cgic clean 		### -[o] directory depends on config file.

	rm -f libCGIDebugLogc.a
	$(MAKE) --directory=$(Libraries) clean_all

	rm -f libflate.a
	$(MAKE) --directory=$(Flate) clean

	rm -f *.so

