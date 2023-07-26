#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

volatile int stopped = 0;
void handlerUrg(int signo)
{
    stopped = 1;
}

int daemon_func()
{
    pid_t pid = getpid();
    printf("%d\n", pid);

    // goto root directory '/' not to block file systems
    chdir("/");
    // set session identificator setsid (2)
    pid_t sid = setsid();
    // can fail
    for(int i = 0; sid == -1 && i < 10; ++i) {
        sleep(1);
        sid = setsid();
    }
    // close files
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // set SIGURG handler
    signal(SIGURG, handlerUrg);
    
    while(!stopped) {
        usleep(200000);
    }
    return 0;
}

int main(int argc, char ** argv)
{
    pid_t child = fork();
    if(child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(child == 0)
        return daemon_func();

    int wstatus;
    // for daemon
    waitpid(child, &wstatus, WNOHANG);
 
    return 0;
}
