//封装实现一个tcpsocket对象，提供简单的接口
#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BACKLOG 5
class TcpSocket{
public:
    TcpSocket():_sockfd(-1){}
    TcpSocket(const TcpSocket &s)
    :_sockfd(s._sockfd){
    }
    int GetFd(){
        return _sockfd;
    }
    void SetFd(int fd){
        _sockfd=fd;
    }
    //创建套接字
    bool Socket(){
        _sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        if(_sockfd<0){
            perror("socket error\n");
            return false;
        }
        return true;
    }
    void Addr(struct sockaddr_in *addr,const std::string &ip,uint16_t port){
        addr->sin_family=AF_INET;
        addr->sin_port=htons(port);
        inet_pton(AF_INET,ip.c_str(),&(addr->sin_addr.s_addr));
    }
    //绑定地址信息
    bool Bind(const std::string &ip,const uint16_t port){
        struct sockaddr_in addr;
        Addr(&addr,ip,port);
        socklen_t len=sizeof(struct sockaddr_in);
        int ret=bind(_sockfd,(struct sockaddr*)&addr,len);
        if(ret<0){
            perror("bind error\n");
            return false;
        }
        return true;
    }
    //服务端开始监听
    bool Listen(int backlog=BACKLOG){
        int ret=listen(_sockfd,backlog);
        if(ret<0){
            perror("listen error\n");
            return false;
        }
        return true;
    }
    //客户端发起连接请求
    bool Connect(const std::string &ip,const uint16_t port){
        struct sockaddr_in addr;
        Addr(&addr,ip,port);
        socklen_t len=sizeof(struct sockaddr_in);
        int ret=connect(_sockfd,(struct sockaddr*)&addr,len);
        if(ret<0){
            perror("connect error\n");
            return false;
        }
        return true;
    }
    //服务端获取新建连接
    bool Accept(TcpSocket *sock,std::string *ip,uint16_t *port){
        struct sockaddr_in addr;
        socklen_t len=sizeof(struct sockaddr_in);
        int clisockfd=accept(_sockfd,(struct sockaddr*)&addr,&len);
        if(clisockfd<0){
            perror("accpet error\n");
            return false;
        }
        sock->_sockfd=clisockfd;
        if(ip!=NULL){
            *ip=inet_ntoa(addr.sin_addr);
        }
        if(port!=NULL){
            *port=ntohs(addr.sin_port);
        }
        return true;
    }
    //发送数据
    bool Send(const std::string &data){
        int ret=send(_sockfd,data.c_str(),data.size(),0);
        if(ret<0){
            perror("send error\n");
            return false;
        }
        return true;
    }
    //接收数据
    bool Recv(std::string *buf){
        char tmp[4096]={0};
        int ret=recv(_sockfd,tmp,4096,0);
        if(ret<0){
            perror("recv error\n");
            return false;
        }
        else if(ret==0){
            printf("connect break\n");
            return false;
        }
        buf->assign(tmp,ret);
        return true;
    }
    //关闭套接字
    bool Close(){
        close(_sockfd);
    }
private:
    int _sockfd;
};
