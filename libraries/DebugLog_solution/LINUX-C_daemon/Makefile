



#include ../Makefile.inc

#GEN_EXE = daemon_SIGHUP t_syslog test_become_daemon

#EXE = ${GEN_EXE} ${LINUX_EXE}

#all : ${EXE}

#allgen : ${GEN_EXE}

#clean : 
#	${RM} ${EXE} *.o

#showall :
#	@ echo ${EXE}

#${EXE} : ${LPLIB}		# True as a rough approximation

###
### filename: Makefile
### author  : Joseph Lin
### E-mail  : joseph.lin@aliyun.com
### 
### ------ May/24/2018 08:55  ------
###   v0.0.6 
###   author : Joseph Lin 
###   change : 将 .o 打包成 lib 库， 
###   Note   : 使用 `-L -lbecome_daemon` 的方式来build，会出现函数链接错误
###            使用自动规则， 效果是： `libbecome_daemon.a` 全名在 gcc 命令中，
###            这样目前可以 build 成功。 
###
### ------ May/24/2018 09:46 ------
###   v0.1.1
###   author : Joseph Lin
###   change : 删除编译 test_become_daemon 部分， 因为将该 src 文件夹上移出一层。
###            只编译 become_daemon 库！
###
### ------
###




CC=gcc
RM=rm -f
VPATH=include src lib test_src bin

EXE=test

MACRO_FLAGS = 
## in CGI Debug Log solution use not strict daemon.
#MACRO_FLAGS += -DSTRICT_DAEMON


CPPFLAGS = -I include
CPPFLAGS += $(MACRO_FLAGS)


AR      = ar
ARFLAGS	= rvu
###
### library of basic part:
###
#libbecome_daemon.a: error_functions.o get_num.o become_daemon.o
#	$(AR) $(ARFLAGS) $@ $?

lib/libbecome_daemon.a: libbecome_daemon.a(error_functions.o) libbecome_daemon.a(get_num.o) libbecome_daemon.a(become_daemon.o)

lib/libbecome_daemon.a(error_functions.o): error_functions.o
	$(AR) $(ARFLAGS) $@ $?
lib/libbecome_daemon.a(get_num.o): get_num.o
	$(AR) $(ARFLAGS) $@ $?
lib/libbecome_daemon.a(become_daemon.o): become_daemon.o
	$(AR) $(ARFLAGS) $@ $?



###
### basic part:
###
./bin/error_functions.o: error_functions.c error_functions.h  tlpi_hdr.h ename.c.inc
	${CC} $(CPPFLAGS) -c $< -o $@

./bin/get_num.o:         get_num.c         get_num.h          tlpi_hdr.h  error_functions.h  
	${CC} $(CPPFLAGS) -c $< -o $@

./bin/become_daemon.o:   become_daemon.c   become_daemon.h    tlpi_hdr.h  error_functions.h  get_num.h
	${CC} $(CPPFLAGS) -c $< -o $@






.PHONY: clean del cleanall all help

#all: $(EXE)
#	@echo "====== Finished build \"$(EXE)\" ========="
all: libbecome_daemon.a
	@echo "====== Finished build \"$^\" =========\n"

clean:
	${RM} ./bin/*.o ./lib/*.a
del:
	${RM} ${EXE}

cleanall:
	${RM} ${EXE} ./bin/*.o ./lib/*.a daemonEcho2

install:
	@echo "ERROR: this INSTALL not achieve for now!!!!"


LIB: libbecome_daemon.a
	@echo "\n====== build \"$^\" finished!!!  ========\n"


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

