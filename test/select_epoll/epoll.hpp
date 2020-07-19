#include<cstdio>
#include"tcpsock.hpp"
#include<vector>
#include<sys/epoll.h>
#define MAX_TIMEOUT 3000
class Epoll
{
public:
    Epoll():_epfd(-1){
        _epfd=epoll_create(1);
        if(_epfd<0){
            perror("create epoll error\n");
            exit(-1);
        }
    }
    bool Add(TcpSocket &sock){
        //2.添加描述符监控事件信息
        //获取描述符
        int fd=sock.GetFd();
        //定义描述符对应的事件结构体
        struct epoll_event ev;
        ev.events=EPOLLIN;
        ev.data.fd=fd;
        //添加到内核中
        int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
        if(ret<0){
            perror("epoll ctl error\n");
            return false;
        }
        return true;
    }
    bool Del(TcpSocket &sock){
        int fd=sock.GetFd();
        int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,NULL);
        if(ret<0){
            perror("epoll ctl error\n");
            return false;
        }
        return true;
    }
    bool Wait(std::vector<TcpSocket> *list,int timeout=MAX_TIMEOUT){
        struct epoll_event evs[10];
        int nfds=epoll_wait(_epfd,evs,10,timeout);
        if(nfds<0){
            perror("epoll wait error\n");
            return false;
        }else if(nfds==0){
            printf("epoll wait timeout\n");
            list->clear();
            return true;
        }
        for(int i=0;i<nfds;i++){
            if(evs[i].events&EPOLLIN){
                TcpSocket sock;
                sock.SetFd(evs[i].data.fd);
                list->push_back(sock);
            }
        }
        return true;
    }
private:
    int _epfd;
};

