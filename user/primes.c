#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1


void sieve(int pleft[2]){
    int p;
    read(pleft[RD],&p,sizeof(p));
    if (p==-1)
    {
        /* code */
        exit(0);
    }
    printf("prime %d\n",p);

    int pright[2];
    pipe(pright);

    if(fork()==0){
        close(pright[WR]);
        close(pleft[RD]);
        sieve(pright);
    }else{
        close(pright[RD]);
        int buf;
        while(read(pleft[RD],&buf,sizeof(buf))&&buf!=-1){
            if(buf%p!=0){
                write(pright[WR],&buf,sizeof(buf));
            }
        }
        buf=-1;
        write(pright[WR],&buf,sizeof(buf));
        wait(0);
        exit(0);
    }
    
}
void main(int argc,char **argv)
{
    int input_pipe[2];
    pipe(input_pipe);

    if(fork()==0){
        close(input_pipe[WR]);
        sieve(input_pipe);
        exit(0);
    }else{
        close(input_pipe[RD]);
        int i;
        for ( i = 2; i <=35; i++)
        {
            /* code */
            write(input_pipe[WR],&i,sizeof(i));
        }
        i=-1;
        write(input_pipe[WR],&i,sizeof(i));
        
    }
    wait(0);
    exit(0);
    
}