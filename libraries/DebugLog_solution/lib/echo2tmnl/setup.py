#
#
# ------ 0.0.3 ------
# 使用： `with open(_device, 'wb+',buffering=0 ) as tmnl_fd:`
#
# ------
# 因为使用 "wb+" 出现了： TypeError: a bytes-like object is required, not 'str'
# 所以用回 'w'
# ------
#

from distutils.core import setup

setup (
	name         = 'echo2tmnl',
	version      = '0.0.1',
	py_modules   = ['echo2tmnl'],
	author       = 'joseph',
	author_email = 'joseph.lin@aliyun.com',
	url          = 'none',
	description  = 'none',
	)
