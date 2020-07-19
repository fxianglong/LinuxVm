#include <stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/time.h>

int main()
{
    //1.定义指定事件的描述符集合
    fd_set rfds;
    while(1){
        printf("开始监控\n");
        //开始监控，超时/有就绪则会调用返回,返回的时候将集合中未就绪的描述符移除
        //超时：在tv指定时间内都一直没有描述符就绪
        //有就绪：有描述符的指定的指针
        struct timeval tv;//因为select会重置集合和事件结构，因此每次需要重新设置
        tv.tv_sec=3;
        tv.tv_usec=0;
        FD_ZERO(&rfds);//初始化清空集合
        FD_SET(0,&rfds);//将0号描述符添加到集合中
        int ret=select(0+1,&rfds,NULL,NULL,&tv);
        if(ret<0){
            perror("select error\n");
            return -1;
        }else if(ret==0){
            printf("wait out\n");
            continue;
        }
        if(FD_ISSET(0,&rfds)){
            printf("准备从标准输入读取数据……\n");
            char buf[1024]={0};
            int ret=read(0,buf,1023);
            if(ret<0){
                perror("read error\n");
                FD_CLR(0,&rfds);//移除描述符从集合中
                return -1;
            }
            printf("read buf:[%s]\n",buf);
        }
    }
    return 0;
}

