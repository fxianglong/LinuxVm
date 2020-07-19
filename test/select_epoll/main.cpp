#include"select.hpp"
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
    CHECK_RET(lst_sock.Bind(srv_ip,srv_port));
    CHECK_RET(lst_sock.Listen());
    Select s;
    s.Add(lst_sock);
    while(1){
        std::vector<TcpSocket> list;
        bool ret=s.Wait(&list);
        if(ret==false){
            return -1;
        }
        for(auto sock:list){
            //遍历就绪的Tcpsocket
            if(sock.GetFd()==lst_sock.GetFd()){
                TcpSocket new_sock;
                ret=lst_sock.Accept(&new_sock);
                if(ret==false){
                    continue;
                }
                s.Add(new_sock);
            }
            else{
                std::string buf;
                ret=sock.Recv(&buf);
                if(ret==false){
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

