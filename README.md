# Embedded-GUI-Develop
嵌入式LINUX 的 Web-GUI 在LINUX主机上开发（CGI）--使用mini-httpd，便于编译和可以直接移植。

> 本文档将最新内容放在上部。按日期从早期到现在对应内容为自下向上。



**注：推荐本地使用 Typora 打开 README.md**



*Overview*

[TOC]

## Contents

  **master**

```
------------- README.md  -----------
Embedded-GUI-Develop
    Contents
    Summary
    ToDo
    ChangeLog
        Aug/05/2018
        Aug/04/2018
        2018/Jul/22
        2018/Jul/21
        daemonEcho2
        2018/Jul/09
        2018/Jul/08
        2018/Jul/07
        2018/Jul/03 -- mini_httpd
        2018/Jul/01 -- CGIDebugLog
    Note
```



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
$ 
$ #### 1. 运行 mini_httpd  ######
$ ## a. 需要修改 mini_httpd.conf 文件
$ ## b. 运行 mini_httpd 命令
$ ## a. & b. 查看 "2018/Jul/22 -> usage" 一节
$
$ #### 2. 运行 CGIDebugLogd.py 在后台 
$ cd WorkPath/ && ./bin/DaemonEcho2 start `tty` && cd ../
$ 
$ ## 切换 输出终端方式：
$ cd WorkPath/ && ./bin/DaemonEcho2 restart <tty> && cd ../
$   ## <tty> should like: /dev/ttyS0 or /dev/pts/1 etc..  
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



## ToDo

- [x] 通过打 patch 的方式修改，而非在 source code 里面修改，或者Makefile；daemonEcho2.c 和 htpasswd.c

    2018/Jul/03 build_patch.py fix htpasswd.c build.

- [x] CGIDebugLogd 使用 python-daemon solution
- [ ] flate API 向下兼容
- [ ] README.md 的 `$ make Include` 说明！
- [ ] make install 的 python install  和 *.cgi; *.html install 区分。



## **Note**

​    N/A



## Change Log

### Aug/05/2018

**fixed** CGIDebugLogd 使用 python-daemon solution

使用 python-daemon 同时 **fixed** 重新分配 tty 的问题！

  r460 - r466

1. 因为 python-daemon 库在 WorkPath/lib/python3.5/ 中， 所以启动 DaemonEcho2 **必须严格** 路径！
2. 启动 DaemonEcho2 命令： `WorkPath/ $ ./bin/DaemonEcho2 start <tty>` (如： ./bin/DaemonEcho2 "/dev/pts/1" )
3. **python-daemon 库的 runner.py 有bug， 做了 fix 在上述路径中！**
4.   重新分配 tty 命令(Example)： `WorkPath/ $ ./bin/DaemonEcho2 restart "/dev/ttyS0"`



### Aug/04/2018

1. update README.md

2. README.md 使用新结构 -- Contents, Summary, ToDo, Note, ChangeLog.

     添加 ToDo 内容。(内容 SYNC [GitHub 上的 Issue -> ToDo](https://github.com/RDpWTeHM/Embedded-GUI-Develop/issues/1))

   

### 2018/Jul/22

1. 添加 mini_httpd.conf 的自动适应环境patch!
2. N/A

#### Detail:

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

#### *usage*

a. 修改 mini_httpd.conf 文件使适应主机运行环境。

方式一， 使用 mini_httpd.patch：

```shell
$ ## 按上述 Detail -> 使用 jinja2 库， 安装 jinja2 库。
$ ./WorkPath/etc/mini_httpd/mini_httpd.patch
$ 
```

方式二， 手动修改：

```shell
$ cp ./WorkPath/etc/mini_httpd/mini_httpd.conf.Reference ./WorkPath/etc/mini_httpd/mini_httpd.conf
$ ## 修改 mini_httpd.conf 中的路径。
```

b. 运行 mini_httpd 

```shell
$ ./WorkPath/bin/mini_httpd -C ./WorkPath/etc/mini_httpd/mini_httpd.conf
```





### 2018/Jul/21

   merge 了 Jul-19-ToDo 这个branch 的 develop，

1. 添加了 CGIC 库
2. 添加了 Flate 库
3. daemonEcho2 进程的相关启动做了修改。

### daemonEcho2

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



### 2018/Jul/09

#### *.cgi 使用 libCGIDebugLogc.a 测试

测试文件为： `src/hello-CGIDebugLog.c` , 

编译出 *.cgi：

  `src/ $ gcc hello-CGIDebugLog.c ../lib/libCGIDebugLogc.a -o hello-CGIDebugLog.cgi` 

手动安装：`/ $ mv ./src/hello-CGIDebugLog.cgi ./WorkPath/var/www/cgi-bin/` 

打开浏览器测试（在mini_httpd 和 CGIDebugLogd.py 正常运行的情况下）。



### 2018/Jul/08

#### 新增 CGIDebugLogc 测试

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



### 2018/Jul/07

#### mini_httpd + CGI 运行测试

> 添加 HTML-hello.c -> HTML-hello.cgi 测试 Web server 环境

  .../src/ $ gcc HTML-hello.c -o HTML-hello.cgi
  .../src/ $ cp HTML-hello.cgi ../WorkPath/var/www/cgi-bin/

  使用浏览器 http://localhost:8282/cgi-bin/HTML-hello.cgi 运行测试



#### mini_httpd.conf 配置文件--mini_httpd运行环境

  查看 WorkPath/README.md



#### 避免重复编译

基本使用：

```shell
$ ## root 路径
$ make init 
$ make lib
$ make
$
```

依赖库（CGIDebugLog，mini_httpd）在文件夹下，因为文件夹时间戳和文件的时间戳有些不同，所以看起来应该需要一些妥协方案。



### 2018/Jul/03 -- mini_httpd

```shell
$ cd libraries
$ make
$ ls -lF
mini_httpd*
CGIDebugLogd.py
daemonEcho2*
...
```



### 2018/Jul/01 -- CGIDebugLog

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


