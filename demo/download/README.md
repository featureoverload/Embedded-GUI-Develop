# README



- 加入 CGI Debug  Solution - 使用 socket。





## Run Demo

```shell
#### terminal 1 ####
$ tty
/dev/pts/1
$ pwd
/<path>/<to>/demo/download/
$ ls
CGIDebugLogd.py     echo2tmnl.py

$ python3 CGIDebugLogd.py /dev/pts/1
Serving DebugLog print on port 21919...

```

```shell
#### terminal 2 ####
$ ls
cgi_download.html    download.c
utils.c              utils.h
Note_Replace-ME_as_CGIC-source-code-dir.txt

## Note: /\ only list the usefull files at this time(not include *.py)

$ tar -xzvf ../../libraries/cgi/cgi207.tar.gz -C ./
$ make
...
$ ls
(new) utils.o download.cgi
$ sudo make install
...
```

使用浏览器打开 `http://localhost/cgi-bin/download.cgi` 可以测试（会看到 log 正常输出到第一个终端）。





