# Head

reserve place

*Overview*

[TOC]

## Contents

  N/A



## 2018/Jul/22

1. 添加 mini_httpd.conf 的自动适应环境patch!
2. N/A

**Detail:**

*1. mini_httpd.conf patch*

​    使用了 jinja2 库，通过模板修改 mini_httpd.conf.jinja2 并创建一个新文件 mini_httpd.conf，将修改得到的数据输出到 minni_httpd.conf 文件中。

​    使用 jinja2 库：

```shell
$ python3 -c "import jinja2"          ## 测试主机环境中是否有 jinja2 库
$
$ sudo apt-get install python3-pip    ## 如果还未安装过 pip 的话执行这条命令。
$ 
$ sudo pip3 install --upgrade jinja2  ## 安装 jinja2 库。
$
```



