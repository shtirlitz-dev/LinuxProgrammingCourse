// makes a daemon

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>


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
    // close files stdio - 
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    while(1) {
        sleep(3);
    }
    return 0;
}

int main(int argc, char ** argv)
{
    //pid_t pid = getpid();
    //printf("initial %d\n", pid);

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
 
    // for a normal child
    /*
       wait(&wstatus);

        if (WIFEXITED(wstatus)) {
            // Child process terminated normally
            int exit_code = WEXITSTATUS(wstatus);
            printf("Child process exited with code: %d\n", exit_code);
        } else {
            // Child process terminated abnormally (e.g., by a signal)
            printf("Child process terminated abnormally\n");
        }
    */
    //printf("parent exit\n");
    return 0;
}
