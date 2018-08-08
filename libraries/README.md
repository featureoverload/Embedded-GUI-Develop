# libraries 文件夹下相关说明

reserve place

*Overview*

[TOC]

## Contents

  N/A



## build out mini_httpd

#### mini_httpd-1.19.tar.gz

mini_httpd 的 source code， 通过 `$ make init` 解压出来给编译使用。

#### build_patch.py

在 LINUX 主机上编译 mini_httpd 的source code 会提示编译错误，因为一个函数与标准库冲突（重复定义）。

运行该 patch 修改该冲突函数名，然后即可编译通过。 



## cgic - for build out *libcgic.a*

#### cgic/ 文件夹 

​    该文件夹下放置 cgic 的各个版本的 source code，在 lib/cgic/ 文件夹下已经放置了 cgic 2.01 版的 source code，默认情况下无需使用该目录下的压缩文件夹。



## DebugLog_solution

​    因为 mini_httpd 是daemon 进程 (后台进程)，而 mini_httpd 调用的 CGI 进程是 daemon 进程组的成员进程；mini_httpd 是组长进程。

​    因为 LINUX 主机的进程组策略（终端策略？）； 所以， CGI 进程将无法获得任何终端的描述符，即无法写数据到终端（对于有操作权限的文件，读写文件没有问题）。

​    使用另外一个 daemon 进程作为 TCP server， CGI 进程使用 Debug Log solution 的 API，该 API 内部是 TCP client， 向该 daemon 进程发送想要显示在终端的 debug 信息。



#### DebugLog_solution.zip

​    该压缩文件内代码已经 out of date， 将不再使用。



#### DebugLog_solution 文件夹

​    该文件夹最开始由 DebugLog_solution.zip 解压出来，fix 一些小bug， 做了一些新 solution。

该 Debug Log 的 solution 使用这个文件夹中的 source code 编译。





