#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0 //read
#define WR 1//write
int main(int argc,char const *argv[]){
    char buf='P';

    int fd_s2f[2];
    int fd_f2s[2];
    pipe(fd_f2s);
    pipe(fd_s2f);

    int pid=fork();
    int exit_status=0;

    if(pid<0)//fall
    {
        printf("fork() error!\n");
        close(fd_f2s[RD]);
        close(fd_f2s[WR]);
        close(fd_s2f[RD]);
        close(fd_s2f[WR]);
        exit(1);

    }else if(pid==0){ //son
        close(fd_f2s[WR]);
        close(fd_s2f[RD]);
        
        if(read(fd_f2s[RD],&buf,sizeof(char))!=sizeof(char)){
            printf("child read() error!\n");
            exit_status=1;
        }else{
            printf("%d: received ping\n",getpid());
            buf='T';
        }

        if(write(fd_s2f[WR],&buf,sizeof(char))!=sizeof(char))
        {
            printf("child write() error!\n");
            exit_status=1;
        }


        close(fd_f2s[RD]);
        close(fd_s2f[WR]);
        
        exit(exit_status);

    }else{ //father
        close(fd_f2s[RD]);
        close(fd_s2f[WR]);

        if (write(fd_f2s[WR],&buf,sizeof(char))!=sizeof(char))
        {
            /* code */
            printf("father write() error!\n");
            exit_status=1;
        }
        
        if(read(fd_s2f[RD],&buf,sizeof(char))!=sizeof(char)){
            printf("father read() error!\n");
            exit_status=1;
        }else{
            printf("%d: received pong\n",getpid());
        }

        close(fd_f2s[WR]);
        close(fd_s2f[RD]);

        exit(exit_status);

    }
    return 0;
}