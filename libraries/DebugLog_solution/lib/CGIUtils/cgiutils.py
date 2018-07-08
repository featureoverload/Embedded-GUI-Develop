#! /usr/bin/python3

"""
#
# file name: cgiutils.py
# author   : Joseph Lin
# E-mail   : joseph.lin@aliyun.com
#
# =======================================
# @copyright
#
# ======================================
#
"""

###
### import packages
###
import sys
import os

import socket

###
### Global variables
### 
DebugFlag = False

###
### Functions define
###
def CGIPrint( _str, _port ):
	### socket client:
	## reference: runoob.com/python3/python3-socket.html
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	## 获取本地主机名
	hostname = socket.gethostname()

	## 设置服务器监听的端口去连接
	port = int(_port)            ## 字符串转数字 C语言的 atoi() 

	## 连接服务器， 指定主机和端口
	s.connect((hostname, port))

	sendMsg = "%s" %_str        ## $ 0:<appname> 1:<port> 2:<message> 
	s.send(sendMsg.encode('utf-8'))

	s.close()



###
###
###
if __name__ == "__main__":
	argc = len(sys.argv)
	if argc != 3:
		print ( "usage: $ %s <server port> <message>" % sys.argv[0], file=sys.stderr )
		sys.exit(1)
	#fi

	CGIPrint(sys.argv[2], sys.argv[1])

	sys.exit(0)

#fi. 

