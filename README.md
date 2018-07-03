# Embedded-GUI-Develop
嵌入式LINUX 的 Web-GUI 在LINUX主机上开发（CGI）--使用mini-httpd，便于编译和可以直接移植。

*Overview*

[TOC]

## Contents

  N/A

-[x] 通过打 patch 的方式修改，而非在 source code 里面修改，或者Makefile；daemonEcho2.c 和 htpasswd.c

  2018/Jul/03 build_patch.py fix htpasswd.c build.

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
