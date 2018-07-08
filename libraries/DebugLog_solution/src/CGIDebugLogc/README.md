# CGIDebugLogc(库)

  reserve place

*Overview*

[TOC]

## Contents

​    N/A

## v0.0.1 基本

### 手动编译

```shell
$ tree
.
├── CGIDebugLogc.c
├── CGIDebugLogc.h
├── Examples
│   └── main.c
├── include
│   └── web.h
└── Makefile
$ 
$ gcc -c CGIDebugLogc.c -o CGIDebugLogc.o
$ ar rv libCGIDebugLogc.a CGIDebugLogc.o
$ 
$ ## compiler tester
$ gcc -c -I include Examples/main.c -o tester.o
$ gcc tester.o libCGIDebugLogc.a -o tester
$
```

Note: **编译出 tester 必须将 libCGIDebugLogc.a 放在 *.o 的后面！！**

  `$ ./tester` 运行测试前，必须确认 CGIDebugLogd.py 在后台运行。

### 自动编译（Makefile）

```shell
$ make           ## 编译出 libCGIDebugLogc.a
$ make tester    ## 编译出 tester* 测试用例。
```



