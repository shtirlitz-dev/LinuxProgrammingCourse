// makes a daemon

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char ** argv)
{
    pid_t pid = getpid();
    printf("%d\n", pid);
    usleep(1000000);
    return 0;
}
