#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MSGSIZE 16

// //echo hello | xargs echo a b c
// int main(int argc,char *argv[]){
//     char buf[MSGSIZE];
//     read(0,buf,MSGSIZE);//0 fang qian yi ge de biao zhun hua shu ru
//     printf("standardized input:%s\n",buf);//output standardized input:hello;

//     for(int i=0;i<argc;i++){//argc=5
//         printf("argv[%d]:%s\n",i,argv[i]);
//         //output argv[0]:xargs
//                //argc[1]:echo
//                //argv[2]:a
//                //argv[3]:b
//                //argc[4]:c
//     }

//     exec("echo",argv);//output echo a b c
//                       //shi ji zhi xing:echo echo a b c
//     exit(0);

// }

int main(int argc,char *argv[]){
    char buf[MSGSIZE];
    read(0,buf,MSGSIZE);

    char *xargv[MAXARG];
    int xargc=0;
    for(int i=1;i<argc;i++){
        xargv[xargc]=argv[i];
        xargc++;
    }
    char *p=buf;
    for(int i=0;i<MSGSIZE;i++){
        if(buf[i]=='\n'){
            int pid =fork();
            if(pid>0){
                p=&buf[i+1];
                wait(0);
            }else{
                buf[i]=0;
                xargv[xargc]=p;
                xargc++;
                xargv[xargc]=0;
                xargc++;

                exec(xargv[0],xargv);
                exit(0);
            }
        }
    }
    wait(0);
    exit(0);
}