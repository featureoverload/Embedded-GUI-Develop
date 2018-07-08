# Directory Logical

Branch from oldPedant/Trunk/Project/easyPSWD/KeepPSWD/Br_WebServer/v0.1.0@DebugLog...

the from directory has three revision, doesn't matter, it's very basic and easy.

r133, r134, r135 is those revision.



## r156

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



## r157

-[o] socket server 功能。