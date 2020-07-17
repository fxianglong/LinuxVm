#include"tcpsock.hpp"
#include<pthread.h>
#include<algorithm>
#include<sys/wait.h>
#include<sstream>
#include<sys/types.h>
#include<signal.h>
#define CHECK_RET(q) if((q)==false){return -1;}

int main(int argc,char* argv[]){
    if(argc<3){
        printf("am:./tcp_srv 192.168.122.132 9000\n");
        return -1;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip,port));
    CHECK_RET(lst_sock.Listen());
    while(1){
        TcpSocket cli_sock;
        std::string cli_ip;
        uint16_t cli_port;
        bool ret=lst_sock.Accept(&cli_sock,&cli_ip,&cli_port);
        if(ret==false){
            continue;
        }
        std::cout<<"connect success"<<std::endl;
        std::string http_req;
        cli_sock.Recv(&http_req);
        printf("req:[%s]\n",http_req.c_str());
        std::string body="<html><body><h1>hello world!</h1></body></html>";
        std::string blank="\r\n";
        std::stringstream header;
        header<<"Content-Length:"<<body.size()<<"\r\n";
        header<<"Content-Type:text/html\r\n";
        std::string first_line="HTTP/1.1 200 OK\r\n";

        cli_sock.Send(first_line);
        cli_sock.Send(header.str());
        cli_sock.Send(blank);
        cli_sock.Send(body);
        cli_sock.Close();
    }
    //父子进程数据独有，都会具有cli_sock，但是父进程并不同通信
    lst_sock.Close();
    return 0;
}
