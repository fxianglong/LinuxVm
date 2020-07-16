#include"tcpsock.hpp"
#include<algorithm>
#define CHECK_RET(q) if((q)==false){return -1;}
int main(int argc,char* argv[]){
    if(argc!=3){
        printf("em:./tcp_cli 192.168.132.122 9000\n");
        return -1;
    }
    std::string ip=argv[1];
    uint16_t port=atoi(argv[2]);
    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());
    CHECK_RET(cli_sock.Connect(ip,port));
    while(1){
        printf("client say:");
        fflush(stdout);
        std::string buf;
        std::cin>>buf;
        CHECK_RET(cli_sock.Send(buf));

        buf.clear();
        CHECK_RET(cli_sock.Recv(&buf));
        printf("server say:%s\n",buf.c_str());
    }
    cli_sock.Close();
    return 0;
}
