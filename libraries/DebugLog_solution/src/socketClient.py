#!/usr/bin/python3

# file name: socketClient.py
# author   : Huaxing Lin
# E-mail   : joseph.lin@aliyun.com
#
# ------- May/21/2018 22:37 -----
#   v0.0.1 : 基本 function 
#   change : 删除和 TTY device 有关的代码。 
#
# ### SOCKET PROGRAMMING ###
# function : 通过命令行指定 server port，
#            请求通过 port 和server 建立 socket 连接
#            发送通过命令行指定的 message 到 server。 
#            message code: utf-8
#

###
### import pakages
###
import sys

import socket

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
def foo():
	print( "foo function.", file=sys.stdout)
#fed function


###
### running logical:
###
def main():
	argc = len(sys.argv)
	if argc != 3:
		print ( "usage: $ %s <server port> <message>" % sys.argv[0], file=sys.stderr )
		sys.exit(1)
	#fi

	foo()

	### socket client:
	## reference: runoob.com/python3/python3-socket.html
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	## 获取本地主机名
	hostname = socket.gethostname()

	## 设置服务器监听的端口去连接
	port = int(sys.argv[1])            ## 字符串转数字 C语言的 atoi() 

	## 连接服务器， 指定主机和端口
	s.connect((hostname, port))

	sendMsg = "%s" %sys.argv[2]        ## $ 0:<appname> 1:<port> 2:<message> 
	s.send(sendMsg.encode('utf-8'))

	s.close()

	sys.exit(0)
##fed func main


if __name__ == "__main__":
	main()
##END running program.