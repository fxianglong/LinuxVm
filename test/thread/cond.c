#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

int bowl=0;//默认0表示碗中没有饭
pthread_cond_t cook_cond;//实现线程间对blow变量访问的同步操作
pthread_cond_t customer_cond;//实现线程间对blow变量访问的同步操作
pthread_mutex_t mutex;//实现blow变量的访问操作
void* thr_cook(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(bowl!=0){
            pthread_cond_wait(&cook_cond,&mutex);
        }
        bowl=1;
        printf("i make a noddles\n");
        /* sleep(1); */
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&customer_cond);
    }
}

void* thr_customer(void* arg){
    while(1){
        //加锁
        pthread_mutex_lock(&mutex);
        while(bowl!=1){
            pthread_cond_wait(&customer_cond,&mutex);
        }
        bowl=0;//能够走下来表示有饭blow==1
        printf("i eat a noddles\n");
        /* sleep(1); */
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cook_cond);
    }
    return NULL;
}
int main()
{
    pthread_t cook_tid,customer_tid;
    int ret;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cook_cond,NULL);
    pthread_cond_init(&customer_cond,NULL);
    for(int i=0;i<4;i++){
        ret=pthread_create(&cook_tid,NULL,thr_cook,NULL);
        while(ret!=0){
            printf("pthread create error\n");
            return -1;
        }
        ret=pthread_create(&customer_tid,NULL,thr_customer,NULL); 
        if(ret!=0){
            printf("pthread create error\n");
            return -1;
        }
    }
    pthread_join(cook_tid,NULL);
    pthread_join(customer_tid,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cook_cond);
    pthread_cond_destroy(&customer_cond);
    return 0;
}

