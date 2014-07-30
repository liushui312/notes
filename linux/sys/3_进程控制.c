fork函数
#include <unistd.h>
pid_t fork(void);


exec函数
  exec函数并不创建新进程，ID不变，用全新的程序替换了当前进程的正文、数据、
堆和栈。从新程序的main函数开始

#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ... /* (char*)0 */);
int execv(const char *path, char *const argv[]);
int execle(const char *path, const char *arg, ... 
            /* (char*)0, char *const envp[] */);
int execve(const char *filename, char *const argv[], char *const envp[]);
int execlp(const char *filename, const char *arg, ... /* (char*)0 */);
int execvp(const char *filename, char *const argv[]);


当一个进程正常或异常终止时，内核向其父进程发送 SIGCHLD 信号；

#include <sys/types.h>
#include <sys/wait.h>
//阻塞等待任何一个子进程终止，获取状态；若没有子进程，出错；
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);
status: 用来保存子进程状态
options: WCONTONUED 作业控制
         WNOHANG    不阻塞，若不是立即可用，返回0
         WUNTRACED  作业控制
pid: pid = -1 等待任一子进程，于wait相同
     pid > 0  等待ID为pid的子进程
     pid = 0  等待组ID为调用进程组ID的任一子进程
     pid < -1 等待组ID为pid绝对值的任一子进程

成功返回子进程ID，0；失败返回-1

if(WIFEXITD(status))     WEXITSTATUS(status) => 子进程传给exit的低八位
if(WIFSIGNALED(status))  WTERMSIG(status)    => 信号编号(异常)
if(WIFSTOPPED(status))   WSTOPSIG(status)    => 信号编号(暂停)
if(WIFCONTINUED(status)) 

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
