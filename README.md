# Embedded-GUI-Develop
嵌入式LINUX 的 Web-GUI 在LINUX主机上开发（CGI）--使用mini-httpd，便于编译和可以直接移植。

> 本文档将最新内容放在上部。按日期从早期到现在对应内容为自下向上。



*Overview*

[TOC]

## Contents

​    N/A

-[x] 通过打 patch 的方式修改，而非在 source code 里面修改，或者Makefile；daemonEcho2.c 和 htpasswd.c

  2018/Jul/03 build_patch.py fix htpasswd.c build.

## 2018/Jul/07

### mini_httpd + CGI 运行测试

> 添加 HTML-hello.c -> HTML-hello.cgi 测试 Web server 环境

  .../src/ $ gcc HTML-hello.c -o HTML-hello.cgi
  .../src/ $ cp HTML-hello.cgi ../WorkPath/var/www/cgi-bin/

  使用浏览器 http://localhost:8282/cgi-bin/HTML-hello.cgi 运行测试



### mini_httpd.conf 配置文件--mini_httpd运行环境

  查看 WorkPath/README.md



### 避免重复编译

基本使用：

```shell
$ ## root 路径
$ make init 
$ make lib
$ make
$
```

依赖库（CGIDebugLog，mini_httpd）在文件夹下，因为文件夹时间戳和文件的时间戳有些不同，所以看起来应该需要一些妥协方案。



## 2018/Jul/03 -- mini_httpd

```shell
$ cd libraries
$ make
$ ls -lF
mini_httpd*
CGIDebugLogd.py
daemonEcho2*
...
```



## 2018/Jul/01 -- CGIDebugLog

```shell
$ 
$ make lib
$ sudo make install
$ 
$ ./libraries/daemonEcho2 `tty`
$ 
$ ps aux | grep python
$ 
```

CGIDebugLog automatic finished!

**Note**

DebugLog_solution.zip -> src/daemonEcho2.c is different with local SVN trunk now.
