#!/usr/bin/python3

"""
# filename: mini_httpd.patch
# Author  : Joseph Lin
# E-mail  : joseph.lin@aliyun.com
#
#
### ----- 2018/Jul/22 12:30
### v0.0.1
### function: hard-code 路径，使用 jinja2 修改字符串
###
### ------ 2018/Jul/22 13:20
### v0.0.2
### change log: 默认打开的 mini_httpd.conf.jinja2 和 本脚本在同级目录，
###             获取 本脚本 所在的 绝对路径。 (merge r380 solution.)
###
### ------
###
"""

import os
import sys

import jinja2
from jinja2 import Environment, PackageLoader, select_autoescape
from jinja2 import Template


# 209 build-in function error
# 210 command error
# 219 IO Error
# 1   common error
# -1  func error
# 911 bad error
#

doDebug = False

PROJ_DIR = os.environ.get("PROJ_DIR", os.getcwd())
CONF_DIR = os.environ.get("CONF_DIR", "conf")
TEMPLATE_FILE = os.environ.get("TEMPLATE_FILE", "mini_httpd.conf.jinja2")
OUTPUT_FILE = os.environ.get("OUTPUT_FILE", "mini_httpd.conf")


class BuildConfigurationFile():
    def __init__(self, template_file, work_path, output_file):
        self.config_template_file = template_file
        self.work_path = work_path
        self.output_config_file = output_file

    def run(self):
        conf_data = ""
        try:
            with open(self.config_template_file, 'r') as conf_fp:
                conf_data = conf_fp.read()
        except IOError as e:
            print("\noperate mini_httpd.conf.jinja2 file fail!",
                  file=sys.stderr)
            print("error with: %s " % str(e), file=sys.stderr)
            sys.exit(219)

        rootRelativePath = "var/www"
        cgiRelateRootPath = "cgi-bin"
        pidRelativePath = ""
        logRelativePath = "var/log"

        # replace variable of *.conf
        template = Template(conf_data)
        dstConfData = template.render(
            var_mini_httpd_root=self.work_path + "/" + rootRelativePath,
            var_mini_httpd_cgi_path=cgiRelateRootPath,
            var_mini_httpd_pid_path=self.work_path + "/" + pidRelativePath,
            var_mini_httpd_log_path=self.work_path + "/" + logRelativePath)

        if doDebug:
            print("dstConfData: \n%s" % dstConfData, file=sys.stderr)

        try:
            with open(self.output_config_file, 'w') as mini_httpd_fp:
                print("%s" % dstConfData, file=mini_httpd_fp)
        except IOError as e:
            print("\noperate mini_httpd.conf file fail!", file=sys.stderr)
            print("error with: %s " % str(e), file=sys.stderr)
            sys.exit(219)

        sys.exit(0)


def main():
    if doDebug:
        print("running main(): \n", file=sys.stderr)
        print("cwd: %s" % os.getcwd(), file=sys.stderr)

    template_file = '/'.join([PROJ_DIR, CONF_DIR, TEMPLATE_FILE])
    work_path = '/'.join([PROJ_DIR, 'run'])
    output_file = '/'.join([PROJ_DIR, CONF_DIR, OUTPUT_FILE])
    obj = BuildConfigurationFile(template_file, work_path, output_file)
    obj.run()


if __name__ == '__main__':
    main()
