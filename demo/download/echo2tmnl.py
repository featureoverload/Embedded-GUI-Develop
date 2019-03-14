#!/usr/bin/python3

#
# ttyS0 不是 Serial
#
# pts/X 和 ttyS0 对于其它组内用户，是可写不可读的。
# 同时不能使用 append 打开，因为指针位置也是不可获取的。
#
# --对于出错处理！ 要看使用的环境。
#   daemon 进程调用了本 function ，基本上就是会执行 except，
#  print 也并不能输出给USER 看到！！
#

###
### import pakages
###
import sys

###
### global variables
##

##
# for Debug
##
doThis = 2


###
# function:
###
def echo2ttyS0(_str):
    try:
        with open("/dev/ttyS0", "w") as ttyS0_fd:
            print(_str, file=ttyS0_fd)
    except IOError as err:
        print("File error: " + str(err))
    # END try...except...
# fed function


def echo2(_device, _str):
    try:
        with open(_device, "w") as tmnl_fd:
            print(_str, file=tmnl_fd)
    except IOError as err:
        print("File error: " + str(err))
    # END try...except...
# fed function


###
# running logical:
###
def main():

    echo2ttyS0("heheda")
    echo2ttyS0("woaini ttyS0")

    argc = len(sys.argv)
    if argc == 2:
        echo2(sys.argv[1], "heheda")
        echo2(sys.argv[1], "woaini ttyS0")
    # fi

    sys.exit(0)
# fed func main


if __name__ == "__main__":
    main()
# END running program.
