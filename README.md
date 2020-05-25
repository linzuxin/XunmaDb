# XunmaDb

## XunmaDb的介绍

XunmaDb(寻码图数据库)是个混合数据库，其中包括了键值对数据库，json数据库，sql数据图，图数据库。

* XunmaDb支持直接集成到c/c++程序，只需头文件即可
* XunmaDb支持服务器模式和集群模式和本地模式
* XunmaDb支持脚本语言

## XunmaDb的组成

XunmaDb由下列模块组成

* XmData(寻码数据模块)
  * XmMemory(内存管理)
  * XmFile(文件管理)
* XmSearch(寻码搜索模块)
  * XmIndex(寻码索引)
  * XmIndexCmd(索引指令)
* XmUint(寻码基础模块)
  * XmString(字符串)
  * XmCrypto(寻码加解密)
  * XmProfile(寻码配置模块)
  * XmNumber(数字)
  * XmDateTime(日期)
  * XmCmdParse(命令解析)
* XmScript(寻码虚拟机模块)
  * XmMake
  * XmNASM
  * XmCompiler
  * XmVM
* XmNet(寻码通讯模块)
  * XmSocket
  * XmWebsocket
  * XmHttp
  * XmRpc
  * XmJRpc
* XmBase(寻码基础模块)
  * Xm
  
## XmData