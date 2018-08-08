#!/usr/bin/env python3

"""
# file name: 
# Author   :  
# E-mail   : 
# 
###
###
"""

###
### import packages
###
import os, sys, io


###
### functions define
###
def foo():
	pass


###
### Global variables
###
doDebug = True


###
### running logical
###
def main(argc, argv):
	for i_argv in range(argc):
		print("argv[{}]: {}".format(i_argv, argv[i_argv]))


if __name__ == '__main__':
	main(len(sys.argv), sys.argv)

