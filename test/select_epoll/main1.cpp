#include"epoll.hpp"
#define CHECK_RET(q) if((q)==false){return -1;}
int main(int argc,char* argv[]){
    if(argc!=3){
        printf("usage:./main ip port\n");
        return -1;
    }
    std::string srv_ip=argv[1];
    uint16_t srv_port=atoi(argv[2]);
    
    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    lst_sock.SetNonBlock();//设置套接字为非阻塞
    CHECK_RET(lst_sock.Bind(srv_ip,srv_port));
    CHECK_RET(lst_sock.Listen());
    Epoll e;
    e.Add(lst_sock);
    while(1){
        std::vector<TcpSocket> list;
        bool ret=e.Wait(&list);
        if(ret==false){
            return -1;
        }
        for(auto sock:list){
            //遍历就绪的Tcpsocket
            if(sock.GetFd()==lst_sock.GetFd()){
                TcpSocket new_sock;
                new_sock.SetNonBlock();
                ret=lst_sock.Accept(&new_sock);
                if(ret==false){
                    continue;
                }
                e.Add(new_sock);
            }
            else{
                std::string buf;
                ret=sock.Recv(&buf);
                if(ret==false){
                    e.Del(sock);
                    sock.Close();
                    continue;
                }
                printf("client say:%s\n",buf.c_str());
                buf.clear();
                printf("server say:");
                fflush(stdout);
                std::cin>>buf;
                ret=sock.Send(buf);
                if(ret==false){
                    sock.Close();
                    continue;
                }
            }
        }
    }
    lst_sock.Close();
    return 0;
}

