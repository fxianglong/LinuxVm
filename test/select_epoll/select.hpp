//封装一个select类，通过实例化的对象完成select的简单操作
#include<cstdio>
#include<time.h>
#include<vector>
#include<sys/select.h>
#include"tcpsock.hpp"
#define MAX_TIMEOUT 3000
class Select{
public:
    Select():_maxfd(-1){
        FD_ZERO(&_rfds);
    }
    bool Add(TcpSocket &sock){//添加描述符的监控
        //获取到套接字描述符
        int fd=sock.GetFd();
        FD_SET(fd,&_rfds);
        _maxfd=_maxfd>fd?_maxfd:fd;
        return true;
    }
    bool Del(TcpSocket &sock){//删除描述符的监控
        int fd=sock.GetFd();
        FD_CLR(fd,&_rfds);
        if(fd!=_maxfd)
            return true;
        for(int i=_maxfd-1;i>=0;i--){
            if(FD_ISSET(i,&_rfds)){
                _maxfd=i;
                break;
            }
        }
        return true;
    }
    bool Wait(std::vector<TcpSocket> *list,int timeout=MAX_TIMEOUT){//开始监听，转向外部返回就绪的TcpSocket
        //select 开始监控
        //判断哪些描述就绪了，组织TcpSocket对象，添加到list中
        struct timeval tv;
        tv.tv_sec=timeout/1000;
        tv.tv_usec=(timeout%1000)*1000;
        fd_set tmp_set=_rfds;//每次使用临时的集合进行监控
        int ret=select(_maxfd+1,&tmp_set,NULL,NULL,&tv);
        if(ret<0){
            perror("select error\n");
            return false;
        }
        else if(ret==0){
            list->clear();
            /* printf("wait timeout\n"); */
            return true;
        }
        //0~maxfd逐个进行判断那个数字在哪个集合中那个数据就是就绪的描述符的值
        for(int i=0;i<=_maxfd;i++){
            if(!FD_ISSET(i,&tmp_set)){
                continue;
            }
            TcpSocket sock;
            sock.SetFd(i);
            list->push_back(sock);
        }
        return true;
    }
private:
    fd_set _rfds;//可读事件的描述符集合
    int _maxfd;//集合操作每次的最大描述符
};
