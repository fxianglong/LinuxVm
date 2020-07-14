#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<cstdio>
#include<queue>
#include<pthread.h>

typedef void (*handler_t)(int);
class ThreadTask{
public:
    ThreadTask(){}
    void SetTask(int data,handler_t handler){
        _data=data;
        _handler=handler;
    }
    void Run(){//外部处理只需要调用Run
        return _handler(_data);
    }
private:
    int _data;//任务中要处理的数据
    handler_t _handler;//任务中处理数据的方法
};
#define MAX_THREAD 3
class ThreadPool{
public:
    ThreadPool(int max_thr=MAX_THREAD)
        :_thr_max(max_thr)
    {
        pthread_mutex_init(&_mutex,NULL);
        pthread_cond_init(&_cond,NULL);
        for(int i=0;i<_thr_max;i++){
            pthread_t tid;
            int ret=pthread_create(&tid,NULL,thr_start,this);
            if(ret!=0){
                printf("create pthread error\n");
                exit(-1);
            }
        }
    }
    ~ThreadPool(){
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }
    bool TaskPush(ThreadTask &task){
        pthread_mutex_lock(&_mutex);
        _queue.push(task);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_broadcast(&_cond);//入队后唤醒所有线程，谁抢到谁处理
        return true;
    }
    static void* thr_start(void* arg){
        //不断的从任务队列中取出任务，执行任务的Run接口就可以
        ThreadPool *p=(ThreadPool*)arg;
        while(1){    
            pthread_mutex_lock(&p->_mutex);
            while(p->_queue.empty()){
                pthread_cond_wait(&p->_cond,&p->_mutex);
            }
            ThreadTask task;
            task=p->_queue.front();
            p->_queue.pop();
            pthread_mutex_unlock(&p->_mutex);
            task.Run();//任务的处理要放在解锁之外，因为当前的锁保护的是队列的操作
        }
        return NULL;
    }
private:
    int _thr_max;
    std::queue<ThreadTask> _queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};
