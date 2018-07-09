# Work Path
 运行目录

*Overview*

[TOC]

## Contents

  N/A

## basic usage

```shell
 $ ######    make mini_httpd work   #######
 $ cd <Path>/github-Embedded-GUI-Develop
 $ cp ./libraries/mini_httpd ./WorkPath/bin/ 
 $ cp ./WorkPath/etc/mini_httpd/mini_httpd.config ./WorkPath/etc/mini_httpd/mini_httpd.conf
 $ vim ./WorkPath/etc/mini_httpd/mini_httpd.conf 
 $ ## 将 .conf 文件的路径改成实际环境的工作路径。 
 $
 $ cd <Path>/github_Embedded-GUI-Develop/WorkPath
 $ sudo ./bin/mini_httpd -C ./etc/mini_httpd/mini_httpd.conf
 $ ## ^ 用 root 权限运行好一点。
 $ 
```

