#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MSGSIZE 16

// echo hello too | xargs echo bye
int main(int argc, char *argv[])
{
    //Q1怎么获取前一个命令的标准化输出（即此命令的标准化输入）?
    char buf[MSGSIZE];
    read(0,buf,MSGSIZE);
    //printf("获取到的标准化输入: %s\n",buf);
    //Q2如何获取到自己的命令行参数
    char *xargv[MSGSIZE];
    int xargc = 0;
    for(int i = 1;i<argc;i++)
    {
        xargv[xargc] = argv[i];
        xargc++;
    }

    char *p = buf;

    for (int i = 0;i<MSGSIZE;i++)
    {
        if(buf[i]=='\n')
        {
            int pid = fork();
            if((pid>0))
            {
                p = &buf[i+1];
                wait(0);
            }
            else
            {    //Q3如何使用exec去执行命令
                buf[i] = 0;
                xargv[xargc] = p;
                xargc++;
                xargv[xargc] = 0;
                xargc++;
    
                exec(xargv[0],xargv);
                exit(0);
            }
        }

    }
    wait(0);

    exit(0);
}