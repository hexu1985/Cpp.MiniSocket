## 一个简单的C++网络编程库, 封装了windows socket API和BSD socket API

### 其特性包括：

- 跨平台, 同时支持windows和linux平台
- 易用性, windows平台不需要WSAStartup之类的全局初始化函数
- 同时支持IPv4和IPv6

### 编译项目：

```shell
$ cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX="install" -DCMAKE_BUILD_TYPE=Release  
$ cmake --build build --target install  
```

### API文档：

API文档是通过doxygen生成的，需要事先安装doxygen，然后通过如下命令生成html格式的API文档：

```shell
$ cd doc
$ ./build_doc.sh
```
