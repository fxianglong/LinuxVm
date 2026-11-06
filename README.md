# Linux 系统编程练习 LinuxVm

![C](https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=black)
![C++](https://img.shields.io/badge/C++-00599C?logo=cplusplus&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)

在 Linux 虚拟机上学习系统编程时写的练习代码，覆盖进程、进程间通信、多线程同步、网络编程和 I/O 多路转接。每个目录都是独立的小例子，可以单独编译运行。

## 目录

| 目录 / 文件 | 主题 | 内容 |
|---|---|---|
| `test/minishell.c` | 进程控制 | 迷你 shell：读取命令、`fork` + `exec` 执行，支持输出重定向 |
| `test/process/` | 进程 | 进程创建、父子进程关系 |
| `test/pipo1.c` | 进程间通信 | 匿名管道 |
| `test/thread/` | 多线程 | 线程创建、互斥锁抢票、条件变量与信号量实现的生产者-消费者、线程池 `ThreadPool.hpp` |
| `test/socket/udp_*.c` | 网络编程 | UDP 客户端 / 服务端 |
| `test/socket/tcp/` | 网络编程 | TCP 封装 `tcpsock.hpp`，单执行流、多进程、多线程三种服务端 |
| `test/socket/http/` | 网络编程 | 基于 TCP 的简易 HTTP 服务器 |
| `test/select_epoll/` | I/O 多路转接 | `select` 与 `epoll` 封装及对应的 TCP 服务器 |
| `tests/` | 其他 | C++ 小练习 |

## 编译运行

带 `makefile` 的目录直接 `make`，其余单文件例子：

```bash
gcc test/minishell.c -o minishell && ./minishell
g++ test/select_epoll/main.cpp -o epoll_srv && ./epoll_srv
g++ test/thread/main.cpp -o pool -lpthread && ./pool
```

## 学习笔记

对应的知识点整理在 CSDN 的 [Linux](https://blog.csdn.net/weixin_44826356) 和「网络原理」专栏：[海盗船长y 的博客](https://blog.csdn.net/weixin_44826356)
