## 一个简单的C++网络编程库, 封装了windows socket API和BSD socket API
### 其特性包括:
* 跨平台, 同时支持windows和linux平台
* 易用性, windows平台不需要WSAStartup之类的全局初始化函数
* 同时支持IPv4和IPv6
### 编译方法:
> $ cmake -Bstage -H. -DCMAKE_INSTALL_PREFIX="install" -DCMAKE_BUILD_TYPE=Release  
> $ cmake --build stage --target install  
--------------------------------------------------------------------------------
[API文档](https://hexu1985.github.io/computer/my_project/mini_socket/index.html)
