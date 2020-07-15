//编写一个udp服务端接口程序
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>//sockaddr结构体/IPPROTO_UDP
#include<arpa/inet.h>//包含一些字节序转换的接口
#include<sys/socket.h>//套接字接口文件

int main(int argc,char* argv[])
{
    //argc表示参数个数，通过argv向程序传递端口参数
    if(argc!=3){
        printf("./udp_sev ip port em: ./udp_sev 192.168.172.132 9000\n");
        return -1;
    }
    const char* ip_addr=argv[1];
    uint16_t port_addr=atoi(argv[2]);

    //socket(地址域，套接字类型，协议类型)
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if(sockfd<0){
        printf("create socket error\n");
        return -1;
    }
    //2.绑定地址信息bind(套接字描述符，地址结构，地址长度)
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port_addr);//post是两个字节，所以转换为s短整形
    //htons-将两个字节的主机字节序整数转换为网络字节序的整数
    addr.sin_addr.s_addr=inet_addr(ip_addr);
    //inet_addr将一个点分十进制的字符串IP地址转换为网络字节序的整数IP地址
    socklen_t len=sizeof(struct sockaddr_in);//获取IPV4的长度
    int ret=bind(sockfd,(struct sockaddr*)&addr,len);
    if(ret<0){
        printf("bind error\n");
        return -1;
    }
    while(1){
        char buf[1024]={0};
        struct sockaddr_in cliaddr;
        socklen_t len=sizeof(struct sockaddr_in);
        //recvfrom(描述符，缓冲区，长度，参数，客户端地址信息，地址信息长度)
        //阻塞接收数据，将数据放入buf中，将发送端的地址放入cliaddr中
        int ret=recvfrom(sockfd,buf,1023,0,(struct sockaddr*)&cliaddr,&len);
        if(ret<0){
            perror("recvfrom error\n");
            return -1;
        }
        printf("client say:%s\n",buf);
        fflush(stdout);
        memset(buf,0x00,1024);
        scanf("%s",buf);
        //通过sockfd将buf数据发送到cliaddr客户端
        sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&cliaddr,len);
    }
    close(sockfd);
}

