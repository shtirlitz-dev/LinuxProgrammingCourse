#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void my_sig_handler(int signo)
{
    printf("Called with %d\n", signo);
    abort(); // to terminate
}

int main() {
    signal(SIGINT, my_sig_handler);

    for(int i = 0; ; ++i) 
    {
        printf("Hello %d\n", i);
        sleep(1);
    }
    return 0;
}
