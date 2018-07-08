#!/usr/bin/python3

#
# ttyS0 不是 Serial
# 
# pts/X 和 ttyS0 对于其它组内用户，是可写不可读的。
# 同时不能使用 append 打开，因为指针位置也是不可获取的。
#


###
### import pakages
###
import sys

###
### global variables
##

##
## for Debug
##
doThis = 2


###
### function:
###
def echo2ttyS0(_str):
    try:
        with open("/dev/ttyS0", "w" ) as ttyS0_fd:
            print ( _str, file=ttyS0_fd )
    except IOError as err:
        print ( "File error: " + str(err) )
    #END try...except...
##fed function


###
### running logical:
###
def main():

	echo2ttyS0("heheda")

	echo2ttyS0("woaini ttyS0")

	sys.exit(0)
##fed func main


if __name__ == "__main__":
	main()
##END running program.