#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <fcntl.h>


int update_sum(int fd, int* sum)
{
    char bfr[80];
    int n = read(fd, bfr, sizeof(bfr) -1);
    if(!n) {
        close(fd);
        return 0;
    }
    bfr[n] = 0;
    n = atoi(bfr);
    *sum += n;
    return 1;
}

int main(int argc, char **argv) {
    int fd1 = open("./in1", O_RDWR);
    int fd2 = open("./in2", O_RDWR);
    if (fd1 < 0 || fd2 < 0) 
        return EXIT_FAILURE;

    int sum = 0, n;
    int ok1 = 1, ok2 = 1;
    while(ok1 || ok2)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        if(ok1)
            FD_SET(fd1, &read_fds);
        if(ok2) 
            FD_SET(fd2, &read_fds);

        int max_fd = (fd1 > fd2) ? fd1 : fd2;
        int ready_fds = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (ready_fds == -1) {
            break;
        }

        if (FD_ISSET(fd1, &read_fds)) 
            ok1 = update_sum(fd1, &sum);
        if (FD_ISSET(fd2, &read_fds)) 
            ok2 = update_sum(fd2, &sum);
    }

    printf("%d\n", sum);

    return 0;
}
