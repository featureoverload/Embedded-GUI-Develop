# mini_httpd Develop



## Usage

```shell
$ make init
$ make
$ make runserver
...

$ make stopserver
```



change mini_httpd source code,

```shell
$ make
$ make runserver
...

$ make stopserver
```



## Test

test mini_httpd server could work or not:

```shell
$ cp ../src/py_getEnvir.cgi ./run/var/www/cgi-bin/
$ chmod +x run/var/www/cgi-bin/py_getEnvir.cgi
$ make runserver
...

```

browser access `http://<ip>:8282/cgi-bin/py_getEnvir.cgi`



