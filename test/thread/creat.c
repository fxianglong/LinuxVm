#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
void* thr_start(void* arg){
	while(1){
		printf("hello~%s\n",(char*)arg);
		sleep(1);
	}
}

int main(){
	pthread_t tid;
	char ptr[]="welcome!\n";
	int ret=pthread_create(&tid,NULL,thr_start,(void*)ptr);
	if(ret!=0){
		printf("create pthread error");
	}
	while(1){
		printf("nihao~~");
		sleep(3);
	}
	return 0;
}
