# Embedded-GUI-Develop
嵌入式LINUX 的 Web-GUI 在LINUX主机上开发（CGI）--使用mini-httpd，便于编译和可以直接移植。

> 本文档将最新内容放在上部。按日期从早期到现在对应内容为自下向上。



*Overview*

[TOC]

## Contents

  **master**

-[x] 通过打 patch 的方式修改，而非在 source code 里面修改，或者Makefile；daemonEcho2.c 和 htpasswd.c

  2018/Jul/03 build_patch.py fix htpasswd.c build.

**注：推荐本地使用 Typora 打开 README.md**



## summary

编译和运行总览：

```shell
$ make init
$ make LIB
$ make BIN
$ 
$ sudo make install  #### 安装 python 程序所需的库。
$ 
$ cp bin/* WorkPath/bin/
$ ## 修改 mini_httpd.conf 文件
$ ## 运行 mini_httpd -> 查看 WorkPath/README.md
$ ## 运行 CGIDebugLogd.py 在后台 
$ ##   -> 查看下面的 “2018/Jul/21 -> daemonEcho2 change log” 一节。
$ 
$ ## 运行测试
$ ## 分为 *.c 的 src/ 和 *.html 的 html/
$ ## 分别修改 src/Makefile 和  html/Makefile
$ make src ## or $ make --directory=src
$ make --directory=src install
$ 
$ make html ## or $ make --directory=html
$ make --directory=html install
$
```



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





## 2018/Jul/21

   merge 了 Jul-19-ToDo 这个branch 的 develop，

1. 添加了 CGIC 库
2. 添加了 Flate 库
3. daemonEcho2 进程的相关启动做了修改。

### daemonEcho2 change log

**启动方式：**

```shell
$ ## 运行过 $ cp bin/* WorkPath/bin* 命令之后，
$ ## 启动方式1：
$ ./WorkPath/bin/daemonEcho2 `tty`
$ 
$ ## 或者不copy bin/ 目录下的 daemonEcho2 程序；
$ ## 或者将 daemonEcho2 程序拷贝到其它位置
$ ## 启动方式2：
$ <daemonEcho2 程序所在路径>/daemonEcho2 `tty` <CGIDebugLogd.py 所在路径>/CGIDebugLogd.py
$ 
```

**daemonEcho2 程序说明：**

​    daemonEcho2 至少需要指定输出的终端名，可以使用 `$ tty` 命令查看当前的终端名。

​    默认情况下（只指定输出终端）， daemonEcho2 将假设 CGIDebugLogd.py 所在路径和它同级，然后打开 CGIDebugLogd.py 进程，将其 daemon 化，并且退出自身这个进程。

​    以上述 “启动方式2” 启动，daemonEcho2 将去指定位置运行 CGIDebugLogd.py 进程。 



## 2018/Jul/09

### *.cgi 使用 libCGIDebugLogc.a 测试

测试文件为： `src/hello-CGIDebugLog.c` , 

编译出 *.cgi：

  `src/ $ gcc hello-CGIDebugLog.c ../lib/libCGIDebugLogc.a -o hello-CGIDebugLog.cgi` 

手动安装：`/ $ mv ./src/hello-CGIDebugLog.cgi ./WorkPath/var/www/cgi-bin/` 

打开浏览器测试（在mini_httpd 和 CGIDebugLogd.py 正常运行的情况下）。



## 2018/Jul/08

### 新增 CGIDebugLogc 测试

1. 确保 *2018/Jul/08 -> CGIDebugLogd 运行* 一节中的 CGIDebugLogd.py 在后台运行。

2. 编译出 CGIDebugLogc 的测试用例：

   ```shell
   $ cd libraries/DebugLog_solution/src/CGIDebugLogc/
   $ make
   $ ## ^ 编译出 libCGIDebugLogc.a
   $ 
   $ make tester
   ```

3. 运行测试用例：

   ```shell
   $ pwd
   <path>/Embedded-GUI-Develop/libraries/DebugLog_solution/src/CGIDebugLogc/
   $ ## 在 2. 的路径基础上。
   $ ./tester  ## 可以通过其它终端运行（路径要和上面pwd一样）
   $ ## 可以在运行 CGIDebugLogd.py 打开的终端中看到输出！
   ```


### CGIDebugLogd 运行

  更新过了 daemonEcho2.c 为了适合 WorkPath/ 目录结构

  使 CGIDebugLogd.py 正常运行的命令：

```shell
$ cp ./libraries/daemonEcho2 ./WorkPath/bin/
$ cp ./libraries/CGIDebugLogd.py ./WorkPath/bin/
$ 
$ cd WorkPath
$ ./bin/daemonEcho2 `tty`
```



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

​    N/A
