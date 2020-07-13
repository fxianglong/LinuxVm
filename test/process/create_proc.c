#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
	pid_t ret;
	ret=fork();
	if(ret<0){printf("error");}
	else if(ret==0){printf("this is son");sleep(5);}
	else{
		printf("this is father");
		sleep(10);
	}
	return 0;
}
