# XunmaDb

## XunmaDb的介绍

XunmaDb(寻码图数据库)是个混合数据库，其中包括了键值对数据库，json数据库，sql数据图，图数据库。

* XunmaDb支持直接集成到c/c++程序
* XunmaDb支持服务器模式和集群模式和本地模式
* XunmaDb支持脚本语言

## XunmaDb的组成

XunmaDb由下列模块组成

* data(寻码数据模块)
  * memory(内存管理)
  * model(模型管理)
  * meta(元管理)
  * category(类别管理)
  * file(文件管理)
* search(寻码搜索模块)
  * index(寻码索引)
  * indexcmd(索引指令)
* unit(寻码基础模块)
  * string(字符串)
  * crypto(寻码加解密)
  * profile(寻码配置模块)
  * number(数字)
  * datetime(日期)
  * cmdparse(命令解析)
  * cli
* pattern(寻码模式模块)
  * create
  * structure
  * behavior
* script(寻码虚拟机模块)
  * make
  * nasm
  * compiler
  * build
  * vm
* net(寻码通讯模块)
  * socket
  * websocket
  * http
  * rpc
  * jrpc
* base(寻码基础模块)
  * rbac(权限管理)
  * log(日志管理)
* system(寻码系统模块)
  * boot

## XunmaDb的目录结构

XunmaDb的工程目录结构

|名称|类型|说明|
|--|--|--|
|client|目录|客户端代码|
|server|目录|服务端代码|
|src|目录|源码|
|front|目录|前台|
|vendor|目录|第三方库|
|makefile|文件|make构建文件|
|build.sh|文件|shell构建文件|
|dockfile|文件|dockfile构建文件|
|README|文件|README文件|
