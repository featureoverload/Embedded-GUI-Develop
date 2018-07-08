# Developer Document

检查 Daemon 运行状态：

```shell
$ ./test
$ ps -C test -o "pid ppid pgid sid tty command"
# PID   PPID    PGID    SID  TT  COMMAND
# 24731    1   24730  24730   ?  ./test
```



reference： 《LINUX/UNIX系统编程手册》



## Strict Daemon

严格性的 daemon， PPID, PPID, PGID, SID 等同上，

非严格性的 daemon （只 fork 一次）, 如下：

```shell
$ make
$ ./test
$ ps -C test -o "pid ppid pgid sid tty command"
#    PID   PPID     PGID    SID    TT    COMMAND
#  27448      1  2477448  27448     ?    ./test
```

使用宏开关 STRICT_DAEMON 控制 fork 一次或两次







