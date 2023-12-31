#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int sig1 = 0, sig2 = 0;

// command line: pkill -USR1 solution
void handlerUsr1(int signo)
{
    //printf("Called with %d\n", signo);
    sig1++;
}
// cmd line: pkill -USR2 solution
void handlerUsr2(int signo)
{
    //printf("Called with %d\n", signo);
    sig2++;
}
// cmd line: pkill -TERM solution
void my_sig_handler(int signo)
{
    printf("%d %d\n", sig1, sig2);
    exit(0); // to terminate
}

int main() {
    signal(SIGUSR1, handlerUsr1);
    signal(SIGUSR2, handlerUsr2);
    signal(SIGTERM, my_sig_handler);

    while(1) 
    {
        sleep(10);
    }
    return 0;
}

