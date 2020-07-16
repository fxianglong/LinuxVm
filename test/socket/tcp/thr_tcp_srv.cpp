#include"tcpsock.hpp"
#include<pthread.h>
#include<algorithm>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#define CHECK_RET(q) if((q)==false){return -1;}

void* thr_start(void* arg){
    TcpSocket* cli_sock;
    cli_sock=(TcpSocket*)(arg);
    while(1){
        std::string buf;
        if(cli_sock->Recv(&buf)==false){
            cli_sock->Close();
            pthread_exit(NULL);
            continue;
        }
        printf("client say:%s\n",&buf[0]);
        std::cout<<"server say:";
        fflush(stdout);
        buf.clear();
        std::cin>>buf;

        if(cli_sock->Send(buf)==false){
            cli_sock->Close();
            pthread_exit(NULL);
            continue;
        }
    }
    //循环退出关闭套接字
    cli_sock->Close();
    pthread_exit(NULL);
    return NULL;
}

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
        printf("new connect:[%s:%d]\n",cli_ip.c_str(),cli_port);
        //---------多任务处理
        pthread_t tid;
        TcpSocket cli_socks(cli_sock);
        pthread_create(&tid,NULL,thr_start,(void*)&cli_socks);//线程
        pthread_detach(tid);
    }
    //父子进程数据独有，都会具有cli_sock，但是父进程并不同通信
    lst_sock.Close();
    return 0;
}
