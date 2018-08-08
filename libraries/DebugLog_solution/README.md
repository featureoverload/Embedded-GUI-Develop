# Develop Doc

  reserve place

*Overview*

[TOC]

## Contents
  N/A

## r307 更新

使用方式：

```shell
$ ## checkout code!
$ make
$ sudo make install
$ ./src/daemonEcho2 /dev/ttyS0
$
```


## Merge r156

``` shell
$ svn checkout <url> -r r156
$ cd  <checkout_dir>
$ 
$ cd echo2tmnl/
$ python3 setup.py sdist
$ sudo python3 setup.py install
## ENTER your password
$
$ cd ../
$ chmod +x CGIDebugLogd.py
$ ./CGIDebugLogd.py
## usage: $ ./CGIDebugLogd.py <ternimal device> <contents to echo>
$ ./CGIDebugLogd.py /dev/pts/0 $'hello pts/0'
$
```


## Manual test daemon oled-TIMER

```shell
$ make
$
$ gcc -c ./bin/* runPython-oled.c -o runPython-oled.o
$
$ gcc ./bin/become_daemon.o ./bin/error_functions.o ./bin/get_num.o \
      ./runPython-oled.o \
      -o runPython-oled.out
$
$ # run oled-TIMER backend:
$ ./runPython-oled.out

```



Note: this runPython-oled.c is depend on Evirnment on this RaspberryPi zero-W system.

​     there just for reference.

​     Change directory and install necessary packages.




