#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
int main(){
	while(1){
		printf("[fxl@minishell]$");
		fflush(stdout);
		char buf[1024]={0};
		fgets(buf,1023,stdin);
		buf[strlen(buf)-1]='\0';
		//解析重定向
		char*ptr=buf;
		int redirect_flag=0;
		char* redirect_file=NULL;
		while(*ptr!='\0'){
			if(*ptr=='>'){
				redirect_flag=1;//清空重定向
				*ptr='\0';//将>替换重接为标志服
				ptr++;
				if(*ptr=='>'){
					redirect_flag=2;//追加重定向
					ptr++;				
				}
				while(*ptr==' '&&*ptr!='\0')ptr++;
				redirect_file=ptr;
				while(*ptr!=' ')ptr++;
				*ptr='\0';
			}
			ptr++;}
		char* argv[32]={NULL};
		int argc=0;
		ptr=buf;
		while(*ptr!='\0'){
			if(*ptr!='\0'){
				argv[argc]=ptr;
				argc++;
				while(*ptr!=' '&&*ptr!='\0'){
					ptr++;}
				*ptr='\0';
			}
			ptr++;
		}
		argv[argc]=NULL;
		pid_t pid=fork();
		if(pid==0){
			if(redirect_flag==1){
				//清空重定向
				int fd=open(redirect_file,O_WRONLY|O_CREAT|O_TRUNC,0664);
				dup2(fd,1);
			}else if(redirect_flag==2){//追加重定向
				int fd=open(redirect_file,O_WRONLY|O_CREAT|O_APPEND,0664);
				dup2(fd,1);
			}
			execvp(argv[0],(char**)argv);
			perror("execvp error");
			exit(0);
		}
		wait(NULL);
	}
	return 0;
}
