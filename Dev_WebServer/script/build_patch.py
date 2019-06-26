#!/usr/bin/python3

"""
# file name: build_patch.py
#
#
###
###
###
"""

import os
import sys

# import


doDebug = True


# sourceCodePath = os.getcwd()  # /home/josephlin/.../github-EbdGUIDev
# mini_httpd_path = "mini_httpd-1.19"
PROJ_DIR = os.environ.get("PROJ_DIR", os.getcwd())
MINI_HTTPD_SOURCE_DIR = os.environ.get("MINI_HTTPD_SOURCE_DIR",
                                       "mini_httpd-1.19")


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
                tmp_data = data[0:index_head]  # 0...
                tmp_data = tmp_data + "minihttpd_getline"  # 0...minihttpd_getline

                index_tail = data.find(
                    replace_funcName, (index_head + len(replace_funcName)))
                # 0...minihttpd_getline + (...)...
                tmp_data = tmp_data + \
                    data[(index_head + len(replace_funcName)):index_tail]
                # (...)... + minihttpd_getline
                tmp_data = tmp_data + "minihttpd_getline"

                tmp_data = tmp_data + \
                    data[(index_tail + len(replace_funcName)):len(data)]
        except:
            import traceback; traceback.print_exc();
            print("fix_htpasswd_build() replace IO error.", file=sys.stderr)

        try:
            with open(self.htpasswd_fullPath, 'w') as rslt_fp:
                rslt_fp.write("{}".format(tmp_data))
        except:
            import traceback; traceback.print_exc();
            print("fix_htpasswd_build() IO Write Error!", file=sys.stderr)


def main():
    source_code_path = PROJ_DIR + '/' + MINI_HTTPD_SOURCE_DIR
    if doDebug:
        print("mini_httpd source code path: '{}'".format(source_code_path))

    if not os.path.exists(source_code_path):
        print("error mini_httpd source code path not exists!", file=sys.stderr)
        sys.exit(1)

    # fix mini-httpd:
    fix_mini_httpd_build = Mini_httpd(source_code_path)
    fix_mini_httpd_build.fix_htpasswd_build()


if __name__ == "__main__":
    main()
