#!/usr/bin/python3

"""
# file name: build_patch.py
#
#
###
###
###
"""


###
### import packages
###
import os, sys

# import 

###
### Global variables
###
doDebug = True



###
### function define
###
def foo():
	pass
#end foo()

class CGIDebugLog(object):
	def __init__(self):
		pass

	def foo(self):
		pass
#END class.


class Mini_httpd(object):
	def __init__(self, sourceCodePath):
		self.htpasswd_filename = "htpasswd.c"
		self.mini_httpd_rootPath = sourceCodePath
		self.htpasswd_fullPath = self.mini_httpd_rootPath + '/' + self.htpasswd_filename

	def fix_htpasswd_build(self):
		index_head = -1
		index_tail = -1

		replace_funcName = "getline"
		tmp_data = None
		try:
			with open(self.htpasswd_fullPath) as htpasswd_fp:
				data = htpasswd_fp.read()
				
				"""
				0...getline(...)...getline(...)...
				    ^              ^
				    index_head     index_tail
				"""
				index_head = data.find(replace_funcName)
				tmp_data = data[0:index_head] 					## 0...
				tmp_data = tmp_data + "minihttpd_getline" 		## 0...minihttpd_getline

				index_tail = data.find( replace_funcName, (index_head+len(replace_funcName)) )
				## 0...minihttpd_getline + (...)...
				tmp_data = tmp_data + data[(index_head+len(replace_funcName)):index_tail]
				tmp_data = tmp_data + "minihttpd_getline" 		## (...)... + minihttpd_getline

				tmp_data = tmp_data + data[(index_tail+len(replace_funcName)):len(data)]
		except:
			print("fix_htpasswd_build() replace IO error.", file=sys.stderr)


		try:
			with open(self.htpasswd_fullPath, 'w') as rslt_fp:
				rslt_fp.write( "{}".format(tmp_data) )
		except:
			print("fix_htpasswd_build() IO Write Error!", file=sys.stderr)

#END class.


###
### running logical
###
def main():
	sourceCodePath =  os.getcwd() ## /home/josephlin/.../github-EbdGUIDev
	if doDebug:
		print("work path: \"{}\"".format(sourceCodePath))

	## fix mini-httpd:
	mini_httpd_path = "mini_httpd-1.19"
	fix_mini_httpd_build = Mini_httpd(sourceCodePath + '/' + mini_httpd_path)
	fix_mini_httpd_build.fix_htpasswd_build()


if __name__=="__main__":
	main()

