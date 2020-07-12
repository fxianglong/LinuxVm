#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
int main(){
int pipofd[2]={-1};
int ret=pipe(pipofd);
if(ret<0){perror("pipo error");return -1;}
pid_t ps_pid=fork();
if(ps_pid==0){
//子进程
dup2(pipofd[1],1);
execlp("ps","ps","-ef",NULL);
exit(0);
}
pid_t grep_pid=fork();
if(grep_pid==0){
//父进程
close(pipofd[1]);
dup2(pipofd[0],0);
execlp("grep","grep","ssh",NULL);
exit(0);
}
close(pipofd[1]);
close(pipofd[0]);
waitpid(ps_pid,NULL,0);
waitpid(grep_pid,NULL,0);
return 0;
}
