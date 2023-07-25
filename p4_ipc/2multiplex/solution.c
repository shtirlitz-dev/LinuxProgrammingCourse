#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <fcntl.h>


void update_sum(int fd, int* sum)
{
    char bfr[80];
    int n = read(fd, bfr, sizeof(bfr) -1);
    if(n <= 0)
        return;
    bfr[n] = 0;
    n = atoi(bfr);
    *sum += n;
}

int main(int argc, char **argv) {
    int fd1 = open("in1", O_RDWR); // | O_NONBLOCK);
    int fd2 = open("in2", O_RDWR); // | O_NONBLOCK);
    if (fd1 < 0 || fd2 < 0) 
        return EXIT_FAILURE;

    int sum = 0;
    int ok1 = 1, ok2 = 1;
    while(ok1 || ok2)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        if(ok1)
            FD_SET(fd1, &read_fds);
        if(ok2) 
            FD_SET(fd2, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 1; // Set the timeout in seconds
        timeout.tv_usec = 0; // Set the timeout in microseconds (0 for no microseconds)

        int max_fd = (fd1 > fd2) ? fd1 : fd2;
        int ready_fds = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        if (FD_ISSET(fd1, &read_fds)) {
            update_sum(fd1, &sum);
        }
        if (FD_ISSET(fd2, &read_fds)) {
            update_sum(fd2, &sum);
        }
            
        if(ok1) {
            int fd3 = open("in1", O_RDWR); // | O_NONBLOCK);
            if(fd3 != -1)
                close(fd3);
            else
                ok1 = 0;
        }
        if(ok2) {
            int fd4 = open("in2", O_RDWR); // | O_NONBLOCK);
            if(fd4 != -1)
                close(fd4);
            else
                ok2 = 0;
        }
    }

    printf("%d\n", sum);

    return 0;
}
