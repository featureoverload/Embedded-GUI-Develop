#!/usr/bin/python3

"""
#
# ------ 2018/May/07 23:07 ------
#   v0.1.0 
#   change: 加入 Socket, as server.
#   author: Huaxing Lin
#   e-mail: joseph.lin@liyun.com
#   Note  : N/A
#
# ------
#
"""

###
### import pakages
###
import sys

from echo2tmnl import echo2

import socket

###
### global variables
##
HOST,PORT = '',21919

##
## for Debug
##
doThis = False


###
### function:
###
def foo():
	print( "foo function.", file=sys.stdout)
#fed function

def socketInit():
	listen_socket = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
	listen_socket.setsockopt( socket.SOL_SOCKET, socket.SO_REUSEADDR, 1 )
	listen_socket.bind((HOST, PORT))
	listen_socket.listen(1)

	print( "Serving DebugLog print on port %s..." % PORT )
	return listen_socket
#fed function socketInit.

## -[o] 待加入 捕获 Ctrl+C，中断处理函数。


###
### running logical:
###
def main():
	argc = len(sys.argv)
	
	if doThis:
		if argc != 3:
			print( "usage: $ %s <ternimal device> <contents to echo>" %sys.argv[0], file=sys.stderr)
			sys.exit(1)
		#fi

		echo2(sys.argv[1], sys.argv[2])
	#fi doThis.

	foo()					## just for check program running.
	listen_socket = socketInit()

	while True:
		(client_connection, client_address) = listen_socket.accept()

		device_and_msg = client_connection.recv(1024)
		print( device_and_msg, file=sys.stdout ) 
		print ( "type of socket msg: %s\n" %type(device_and_msg))

		#device_and_msg = "%s" %device_and_msg
		device_and_msg = device_and_msg.decode("utf-8")
		print ( "type of socket msg: %s\n" %type(device_and_msg))

		### echo msg which recive from socket client to Terminal.		
		(device, msg) = device_and_msg.split("\r\n", 1)
		print( "device: %s \nmsg: %s" %(device, msg), file=sys.stdout )

		echo2(device, msg)

	#End while.
	
	listen_socket.close()

	sys.exit(0)
##fed func main


if __name__ == "__main__":
	main()
##END running program.
