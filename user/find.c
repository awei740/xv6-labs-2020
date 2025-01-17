#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}

int norecurse(char *path){
    // printf("c %s;\n%s\n",path,fmtname(path));
    char *buf=fmtname(path);
    // printf("norecurse:%s\n",buf);
    if(buf[0]=='.'&&buf[1]==0){
        return 1;
    }
    if(buf[0]=='.'&&buf[1]=='.'&&buf[2]==0){
        return 1;
    }
    return 0;
}

void
find(char *path,char *target)
{
    // printf("%s\n",path);
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
//   printf("111:%s\n",path);
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }
//   printf("222:%s\n",path);
//   printf("st:%s;fd:%s\n",st,fd);
  if(strcmp(fmtname(path),target)==0){
    // printf("fmtname(path):%s path:%s target:%s\n",fmtname(path),path,target);
    printf("%s\n",path);
  }

  switch(st.type){
  case T_FILE:
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    // printf("strcpy:%s\n",buf);
    p = buf+strlen(buf);
    *p++ = '/';
    // printf("strcpy buf:%s\n",buf);
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if(norecurse(buf)==0){
         find(buf,target);
     }
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc==1){
        printf("usage:find [path] [target]\n");
        exit(0);
      }
    if(argc==2){
        find(".",argv[1]);
        exit(0);
      }
    if(argc==3){
        // printf("%s  %s\n",argv[1],argv[2]);
        find(argv[1],argv[2]);
        exit(0);
      }
    exit(0);
}
