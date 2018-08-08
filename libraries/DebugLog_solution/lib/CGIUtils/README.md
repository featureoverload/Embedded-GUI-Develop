# Developer Document

## Introduction

  使用本“库”里面的 function 来实现 python-CGI 的打印 debug log 方式！

## Uage guide

```shell
 $ python3
 ...python3 message...
 >>> 
 >>> ## I am a CGI
 >>> from cgiutils import CGIPrint
 >>> 
 >>> port = 21919  ## socket port connect to server
 >>>
 >>> CGIPrint( "this content will show on a terminal you specify with $ daemonEcho2 <TTY>", port)
 >>>

```


## Install for use

```shell
 <this directory>/ $ 
 <this directory>/ $ python3 setup.py sdist 
 <this directory>/ $ sudo python3 setup.py install
 <this directory>/ $  
```


