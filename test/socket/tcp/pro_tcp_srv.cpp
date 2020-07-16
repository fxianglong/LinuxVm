#include"tcpsock.hpp"
#include<algorithm>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#define CHECK_RET(q) if((q)==false){return -1;}


void sigcb(int signo){
    //当子进程退出的时候就会向父进程发送SIGCHLD信号，毁掉这个函数
    while(waitpid(-1,0,WNOHANG)>0);
}

int main(int argc,char* argv[]){
    if(argc<3){
        printf("am:./tcp_srv 192.168.122.132 9000\n");
        return -1;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);

    signal(SIGCHLD,sigcb);
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
        pid_t pid=fork();
        if(pid==0){
            while(1){
                std::string buf;
                if(cli_sock.Recv(&buf)==false){
                    cli_sock.Close();
                    exit(0);
                }
                printf("client:[%s:%d]say:%s\n",&cli_ip[0],cli_port,&buf[0]);
                std::cout<<"server say:";
                fflush(stdout);
                buf.clear();
                std::cin>>buf;

                if(cli_sock.Send(buf)==false){
                    cli_sock.Close();
                    exit(0);
                    continue;
                }
            }
            cli_sock.Close();
            exit(0);
        }
        //父子进程数据独有，都会具有cli_sock，但是父进程并不同通信
        cli_sock.Close();
    
    }
    lst_sock.Close();
    return 0;
}
