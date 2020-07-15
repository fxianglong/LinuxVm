//封装一个udpsocket类，向外提供简单的接口实现套接字编程
#include <stdio.h>
#include<iostream>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

class UdpSocket{
public:
    UdpSocket():_sockfd(-1){}
    bool Socket(){
        _sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        if(_sockfd<0){

        }
    }
private:
    int _sockfd;
};


int main()
{
    printf("Hello world\n");
    return 0;
}

