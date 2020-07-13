#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

int ticket=100;
pthread_mutex_t mutex;

void* thr_scalpers(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		if(ticket>0){
			printf("i get a ticket %d\n",ticket);
			ticket--;
		pthread_mutex_unlock(&mutex);
		}else{
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
	}
}
int main(){
	pthread_t tid[4];
	pthread_mutex_init(&mutex,NULL);
	for(int i=0;i<4;i++){
		int ret=pthread_create(&tid[i],NULL,thr_scalpers,NULL);
		if(ret!=0){
			printf("thread create error\n");
			return -1;
		}
	}
	for(int i=0;i<4;i++){
		pthread_join(tid[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	return 0;
}
