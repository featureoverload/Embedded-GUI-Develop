#!/usr/bin/env python3

#
# Note:
#     get localhost/cgi-bin/getEvir.py 没有输出， 使用浏览器的 F12 -> network 查看了一下package
#   发现是因为输出不完整的原因。 只是输出到 <meta charset="utf-8">
#   现象和 hello.py 类似， 本来以为是 sys.exit(0) 的原因，误以为它没有真正的好好顺序执行。
#     在这里则觉得会不会是 print 分次太多，所以Apache2 没有等待执行结束就退出。
#   最后发现， 原来是这里的中文原因。 输出的代码中带有中文， 则此调用CGI的进程就死掉了。
#
#     可能是因为运行的平台， raspberry Pi 安装的是标准版， 没有选择中文安装，所以对中文的支持可能有问题。
#   具体原因还有待排查，这边写输出先不要用中文。
#
#    👆 中文输出 soluton: R394
#    Reference: http://www.runoob.com/python3/python3-cgi-programming.html#div-comment-26430
#

###
### rely on packages
###
import os
import sys

import io

###
### global variable
###
# ...
## fix 因为中文输出导致进程死掉
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

###
### funciton define
###
def main():
	### response heard:
	# print ( 'Content-type: text/html')
	# print ()
	str = 'Content-type: text/html' + '\r\n'
	str = str + '\r\n' + '\r\n'

	str = str + "<meta charset=\"utf-8\">"              + '\r\n'
	str = str + "<b>Envirnments avaible(环境变量):</b><br/>"      + '\r\n'
	str = str + "<ul>"                                  + '\r\n'
	for key in os.environ.keys():
		tmp = "<li><span style='color:green'>%30s </span> : %s </li>" % (key, os.environ[key])
		str = str + tmp + '\r\n'
	str = str + "</ul>"                     + '\r\n'

	print (str)
#END func main

###
### program running:
###
if __name__ == "__main__":
	main()
#END running main funciton.
