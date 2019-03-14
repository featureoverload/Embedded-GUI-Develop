#!/usr/bin/env python3

"""
# file name: CGIDebugLogd.py
# author   : Joseph Lin
# E-mail   : joseph.lin@aliyun.com
#
# ------ May/21/2018 23:21 ------
#   v1.0.1
#   change: 加入 socket。
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
HOST, PORT = '', 21919


##
# for Debug
##
doThis = 2


###
# function:
###
def foo():
    print("foo function.", file=sys.stdout)
# fed function


def socketInit():
    listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listen_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listen_socket.bind((HOST, PORT))
    listen_socket.listen(1)

    print("Serving DebugLog print on port %s..." % PORT)
    return listen_socket
# fed function socketInit.

# -[o] 待加入 捕获 Ctrl+C，中断处理函数。


###
# running logical:
###
def main(argc, argv):
    # argc = len(sys.argv)
    # if argc != 3:
    if argc != 2:  # $ <appname> <TTY device>
        print("usage: $ {} <ternimal device>"  # <contents to echo>"
              .format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    listen_socket = socketInit()

    while True:
        (client_connection, client_address) = listen_socket.accept()
        msg = client_connection.recv(1024)  # -[o] 这句代码不是很完善

        msg = msg.decode("utf-8")
        # print("msg recive from client is:\n%s" %msg)
        # ## -[x] 从显示效果来说，需要对发送过来的数据解码
        # echo2(sys.argv[1], sys.argv[2])
        echo2(argv[1], msg)

    listen_socket.close()

    sys.exit(0)
# fed func main


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
# END running program.
