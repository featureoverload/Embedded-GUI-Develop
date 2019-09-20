# OpenWrt Web GUI Develop

==因为个人的水平和精力是有限的，如果本目录下的内容存在错误，疏忽之处，欢迎指出：可创建 Issue 或者 fork 修改后向本仓库做 pull request==

[TOC]

-----

## 理解 Luci 架构

> 注：
>
> 下文“几个元素”中，属于“总结”。
>
> 是在编写本文是为了理清思路，边整理思路边写下的。
>
> 若有不明之处可以在看完本目录下的内容之后再回过头来看一遍。

几个元素：

- 在 OpenWrt 使用 Luci -- Web。

- Luci 使用 lua 语言作为后台。

- Luci 使用 lua 通过 uci 库读取和修改 OpenWrt 协定的 UCI 配置文件。

- OpenWrt 内协定 UCI 配置文件，并提供了不同的接口操作它，其中之一是实现了 lua 语言的 uci 库。

- Luci 框架内基于 lua + uci 库编写了 CBI 框架 -- CBI 框架是 Luci 的子框架。

- CBI 框架加载入 uci 配置文件相应的 lua 模块，对于 HTTP GET 能够以 CBI 框架的运行逻辑将 UCI 配置文件转化渲染成用于 Web 前端显示的 HTML 做 HTTP Response；同样对 HTTP POST 也以 CBI 框架运行的逻辑将 form 表单修改写入到 UCI 配置文件中（和生效）。

- 由于 UCI 有着规范的格式，以及 CBI 框架的 OOP 实现，我们不需要从头如 读取 UCI 配置文件，写出如何渲染成 HTML 的代码再 Response 到 Client（浏览器）；这些操作是可以抽象出来的一套可重用的参数和运行逻辑（方法），用户（promgramer）只需要编写针对细分的不同的配置文件内容的 CBI 模块代码即可（即，OOP 中的 class 已经被实现好了，只需要根据不同的实体内容实例和调整实例内容即可）。

- 因为 CBI 框架的高度可重用性（配置文件大同小异，用户（user）管理网页风格统一），所以我们可以设想只要在编写的 CBI 模块代码中指明该模块关联的 UCI 配置文件，那么 CBI 框架就能够将该 UCI 配置文件以写好的有限的规则读取出来并显示在浏览器上 -- 但是因为我们需要控制配置文件中的哪些部分需要显示，哪些部分不需要显示；哪些部分以何种方式显示，另一些部分又以另外的什么方式显示；所以我们除了声明配置文件外，还需要声明这些内容 -- 这很类似“声明式”编程，后面具体会感觉到。

  （另外一提，这点和 Web 开发的后台管理网站没有本质区别，后台管理网站也是通过编写大量通用的代码涵盖大范围的内容类型（数据库，数据 type）以做到对大部分编写的数据模型无需额外编写管理页面，直接能够以通用的模板显示出来并且能够操作）

- Luci 框架亦是 MVC 模式，其中 CBI 即是 model，因为用户（user）管理网页（路由器的 Web GUI）没有很多花样，所以基本上需要的前端模板 luci 都提供了 -- 即用户编写了 CBI 模块即可，CBI 框架渲染时的运行逻辑能够使用既有的模板为浏览器提供 HTML 显示。当然，在一些需要以特殊方式显示或者提供更丰富的功能的地方，我们仍然可以自己编写模板（view），然后在 CBI 内指定哪些 UCI 内容使用该模板显示。

- Luci 框架的控制器是理解 Luci 框架运行逻辑的关键，它主要将用户编写的控制器规则生成 URI（路由）（并且提供了反向解析的接口），用户（user）只需要在浏览器访问 URL，Luci 就能够通过控制器执行程序中定义的处理代码（比如 CBI 模块）得到处理代码（方法）的返回内容作为 HTTP 响应（View），即 controller -> model -> view（view 中又包含 controller URL）。



**详解 Luci 框架** -- 将分以下步骤进行（可以仔细看一下，有助于理清思路）：

1. 软件结构：源码结构和运行结构
   1. 源码分布介绍和“安装/运行”文件分布
   2. 源码编译/打包方式和上传安装
2. HTTP Server
   1. HTTP Server 和 Luci 交互基本原理
   2. HTTP Server 简单介绍
3. Luci 用户系统
   1. 多用户支持
   2. 状态保持原理（HTTP 是无状态协议）
   3. 登录系统
4. Luci 框架
   1. hello world 级 demo -- 接触与感受
   2. MVC 简介
   3. Controller 和 URI
   4. View -- 模板简介和基本使用/语法
   5. Luci 框架前半部运行逻辑
      1. 从 HTTP Server 到执行 Luci
      2. 从 Luci 入口到 Controller
      3. 从 Controller 到执行 target（函数）
   6. 非 CBI target -- call 与 template 实例
   7. Model（模型 ）-- CBI 框架
      1. CBI 的 hello world 级 demo
      2. 了解 OOP 基本知识和作用
         1. 基本知识
         2. 基本作用 -> 可实现的声明式编程
         3. 加速编程的好处，隐藏大量细节的坏处
         4. lua OOP 语法与动态语法
         5. 类与实例 -- 不容模糊的 `self`
      3. UCI 协议（约定）
         1. 配置文件格式
         2. 常规对应 Web 页面显示的控件
         3. lua 中的 uci 库与 API
      4. CBI 模块实例 - example 页和 example 配置文件
      5. 处理表单 -- 探究 CBI 框架运行逻辑
         1. 模块，节，tab 与 option
         2. 基本类型和动态绑定（组合模式）
         3. lua 脚本代码在什么时候运行？以及 require 和 loader 区别
         4. 运行方法（代码执行逻辑）和重写
         5. 可选的定制化 -- 钩子函数
         6. 配置更新的生效和重定向
   8. 模板扩展 -- javascript 异步请求
5. 节选 - 编程关键词与理解
   1. 类，类型，实例，对象
   2. API，框架，模块，（数据）模型
   3. 钩子，重写
   4. 强类型和弱类型，动态和静态
   5. 面向过程，面向对象，面向声明
   6. UML





> 注：
>
> 因为本目录的内容并没有按照上文“步骤”写完，所以只称得上“粗解”。
>
> 因为 PPT 是先于本文做完的，虽然 PPT 内容本身可能不够精细，但是根据 PPT 内的文字说明，讲解，结合 Luci 源码查看，想必对框架理解会有一定的帮助。
>
> 另外在写 PPT 的时候是为了团队现场讲解，会配置打开的源代码文件讲，所以在 PPT 中主要以概括性、解释性、总结性内容为主，读者需要结合源码看。此处内容后面可能会不定期更新。
>
> 本着不等将事情做到完美的那一刻再分享出来的理由，
>
> > 因为一是这样会过很久之后才分享出来，二是有可能做不到心目中的完美而一直不能分享出来。
>
> 若是有需要的读者早一些看到，并且可能能起到一定的帮助，再由读者自身通过结合源码以及翻阅资料补充便可以理解 Luci 框架。那么本目录下分享出来的内容便是有用的。
>
> 通过我也期望于即使目前此处的内容还很粗糙，但是若有读者能够 fork > 编辑补重小节小段或者是几句话，几行代码解释介绍，那也是甚好的。

粗解 Luci 框架：

### 目录

1. 软件结构：源码结构和运行结构
   1. 源码分布介绍和“安装/运行”文件分布
   2. 源码编译/打包方式和上传安装
2. HTTP Server
   1. HTTP Server 和 Luci 交互基本原理
   2. HTTP Server 简单介绍
3. Luci 用户系统 <font color='red'>《== 未完成</font>
   1. 多用户支持
   2. 状态保持原理（HTTP 是无状态协议）
   3. 登录系统
4. Luci 框架
   1. hello world 级 demo -- 接触与感受
   2. MVC 简介
   3. Controller 和 URI
   4. View -- 模板简介和基本使用/语法
   5. Luci 框架前半部运行逻辑<font color='oren'>《== 参见 [luci框架代码”逻辑”流程图.pdf](luci框架代码"逻辑"流程图.pdf)</font>
      1. 从 HTTP Server 到执行 Luci
      2. 从 Luci 入口到 Controller
      3. 从 Controller 到执行 target（函数）
   6. 非 CBI target -- call 与 template 实例
   7. Model（模型 ）-- CBI 框架
      1. CBI 的 hello world 级 demo
      2. 了解 OOP 基本知识和作用<font color='red'>《== 未完成</font>
         1. 基本知识
         2. 基本作用 -> 可实现的声明式编程
         3. 加速编程的好处，隐藏大量细节的坏处
         4. lua OOP 语法与动态语法
         5. 类与实例 -- 不容模糊的 `self`
      3. UCI 协议（约定）
         1. 配置文件格式
         2. 常规对应 Web 页面显示的控件
         3. lua 中的 uci 库与 API<font color='red'>《== 未完成</font>
      4. CBI 模块实例 - example 页和 example 配置文件
         1. 模块，节，tab 与 option
         2. 基本类型和动态绑定（组合模式）<font color='red'>《== 未完成</font>
         3. input-Value，select-ListValue，checkbox-Flag
      5. 处理表单 -- 探究 CBI 框架运行逻辑
         1. lua 脚本代码在什么时候运行？以及 require 和 loader 区别<font color='red'>《== 未完成</font>
         2. 运行方法（代码执行逻辑）和重写
         3. 可选的定制化 -- 钩子函数<font color='red'>《== 未完成</font>
         4. 配置更新的生效和重定向
   8. 模板扩展 -- javascript 异步请求<font color='red'>《== 未完成</font>
5. 节选 - 编程关键词与理解<font color='red'>《== 未完成</font>
   1. 类，类型，实例，对象
   2. API，框架，模块，（数据）模型
   3. 钩子，重写
   4. 强类型和弱类型，动态和静态
   5. 面向过程，面向对象，面向声明
   6. UML



### 正文

参阅 [luci-web(GUI)-for-develop.pptx](luci-web(GUI)-for-develop.pptx)