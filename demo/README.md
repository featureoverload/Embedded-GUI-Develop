# Demo

快速验证 *.c => *.cgi 功能。



[TOC]

## Apache2 配置

1. CGI mod enable

   ```shell
   $ cd /etc/apache2
   $ sudo vim mods-avaiable/cgi.load
       LoadModule cgi_module /usr/lib/apache2/modules/mod_cgi.so
   +   AddHandler cgi-script .cgi .pl .py .sh
   ~
   ~
   :wq
   $
   $ sudo ln -s /etc/apache2/mods-available/cgid.conf /etc/apache2/mods-enabled/cgid.conf
   $ sudo ln -s /etc/apache2/mods-available/cgid.load /etc/apache2/mods-enabled/cgid.load
   $ sudo ln -s /etc/apache2/mods-available/cgi.load /etc/apache2/mods-enabled/cgi.load
   $
   ```

2. CGI path

   ```shell
   $ cd /etc/apache2
   $ sudo vim conf-available/serve-cgi-bin.conf
   [...]
       	</IfModule>
   
       	<IfDefine ENABLE_USR_LIB_CGI_BIN>
   -   		ScriptAlias /cgi-bin/ /usr/lib/cgi-bin/
   +   		ScriptAlias /cgi-bin/ /var/www/cgi-bin/
   -   		<Directory "/usr/lib/cgi-bin">
   +   		<Directory "/var/www/cgi-bin">
       			AllowOverride None
       			Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
       			Require all granted
       		</Directory>
   [...]
   :wq
   ```

3. Create CGI Path

   ```shell
   $ sudo mkdir -p /var/www/cgi-bin/
   ```

   

4. restart

   ```shell
   $ /etc/init.d/apache2 restart
   ```





------



## Hello World

```shell
$ cd helloworld
$ tar -xzvf /<path>/<to>/cgic207.tar.gz -C ./  # cgi201, cgi205, cgi207 is version
....
$ make all
$ ls -lF
... cgic207/
... helloworld.c
... helloworld.cgi
... libcgic.a
... Makefile
$ sudo make install
```

**访问验证：**

`http://<localhost> or <apache2 server ip>/cgi-bin/helloworld.cgi`



