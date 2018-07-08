#!/usr/bin/python3

#
# this actually be Socket client.
# the CGIDebugLogd.py is Socket Server.
#
# ### SOCKET PROGRAMMING ###
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
	if argc != 4:
		print ( "usage: $ %s <port> <device> <message>" % sys.argv[0], file=sys.stderr )
		sys.exit(1)
	#fi

	foo()

	### socket client:
	## reference: runoob.com/python3/python3-socket.html
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	## 获取本地主机名
	hostname = socket.gethostname()

	## 设置服务器监听的端口去连接
	port = int(sys.argv[1])           ## 字符串转数字 C语言的 atoi() 

	## 连接服务器， 指定主机和端口
	s.connect((hostname, port))

	sendMsg = "%s\r\n%s" %(sys.argv[2], sys.argv[3])
	s.send(sendMsg.encode('utf-8'))

	s.close()

	sys.exit(0)
##fed func main


if __name__ == "__main__":
	main()
##END running program.
