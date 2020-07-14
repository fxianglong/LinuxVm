#include <stdio.h>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<cstdio>
#include<semaphore.h>
#define QUEUE_MAX 5
class RingQueue{
public:
    RingQueue(int maxq=QUEUE_MAX)
        :_queue(maxq),_capacity(maxq)
        ,_step_read(0),_step_write(0){
        sem_init(&_lock,0,1);//用于实现互斥所，只能一个人访问
        sem_init(&_sem_data,0,0);//数据空间计数初始化为0
        sem_init(&_sem_idle,0,maxq);//空闲空间计数初始化为节点个数
        }
    ~RingQueue(){
        sem_destroy(&_lock);
        sem_destroy(&_sem_data);
        sem_destroy(&_sem_idle);
    }
    bool Push(int data){
        //1.判断是否能够访问资源，不能访问则阻塞
        sem_wait(&_sem_idle);//空闲空间计数的判断，空闲空间计数-1
        //2.能访问，则加锁，保护访问过程-空闲空间计数的判断
        sem_wait(&_lock);//lock计数不大于1，当前若可以访问则-1，别人就不能访问了
        //3.资源的访问
        _queue[_step_write]=data;
        _step_write=(_step_write+1)%_capacity;//走到最后，从头开始
        //4.解锁
        sem_post(&_lock);//lock计数+1,唤醒其他加锁阻塞的线程
        //5.入队数据之后，数据空间+1，唤醒消费者
        sem_post(&_sem_data);
        return true;
    }
    bool Pop(int *data){
        sem_wait(&_sem_data);//有没有数据
        sem_wait(&_lock);//有数据则加锁保护访问数据的过程
        *data=_queue[_step_read];//获取数据
        _step_read=(_step_read+1)%_capacity;
        sem_post(&_lock);//解锁操作
        sem_post(&_sem_idle);//取出数据，则空闲空间+1,唤醒生产者
        return true;
    }
private:
    std::vector<int> _queue;
    int _capacity;
    int _step_read;
    int _step_write;
    sem_t _lock;
    sem_t _sem_idle;
    sem_t _sem_data;
};
static int val=0;
void* thr_productor(void* arg){
    RingQueue *queue=(RingQueue*)arg;
    while(1){
        //生产者不断产生数据
        queue->Push(val);
        printf("productor push data:%d\n",val);
        /* sleep(5); */
        val++;
    }
    return NULL;
}
void* thr_customer(void* arg){
    RingQueue *queue=(RingQueue*)arg;
    while(1){
        //消费者不断获取数据
        int data;
        queue->Pop(&data);
        printf("customer get data:%d\n",data);
        /* sleep(5); */
    }
    return NULL;
}
int main()
{
    int ret,i;
    pthread_t ctid[4],ptid[4];
    RingQueue queue;
    for(i=0;i<4;i++){
        ret=pthread_create(&ptid[i],NULL,thr_productor,(void*)&queue);
        if(ret!=0){
            printf("create productor pthread error\n");
            return -1;
        }
    }
    for(i=0;i<4;i++){
        ret=pthread_create(&ctid[i],NULL,thr_customer,(void*)&queue);
        if(ret!=0){
            printf("create productor pthread error\n");
            return -1;
        }
    }
    for(i=0;i<4;i++){
        pthread_join(ptid[i],NULL);
        pthread_join(ctid[i],NULL);
    }
    return 0;
}


