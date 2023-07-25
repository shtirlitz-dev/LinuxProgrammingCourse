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
    if(n <= 0)
        return 0;
    bfr[n] = 0;
    n = atoi(bfr);
    *sum += n;
    return 1;
}

int main(int argc, char **argv) {
    int fd1 = open("in1", O_RDWR); // | O_NONBLOCK);
    int fd2 = open("in2", O_RDWR); // | O_NONBLOCK);
    if (fd1 < 0 || fd2 < 0) 
        return EXIT_FAILURE;

    int sum = 0;
    while(1)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(fd1, &read_fds);
        FD_SET(fd2, &read_fds);

        struct timeval timeout;
        timeout.tv_sec = 8; // Set the timeout in seconds
        timeout.tv_usec = 0; // Set the timeout in microseconds (0 for no microseconds)

        int max_fd = (fd1 > fd2) ? fd1 : fd2;
        int ready_fds = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);

        // select doesn't return when a pipe is closed
        // I assuming that the test don't make a big pause between sending digits
        // so, if timeout is reached, this should mean the test is ended.
        if(!ready_fds)
            break;

        if (FD_ISSET(fd1, &read_fds)) {
            update_sum(fd1, &sum);
        }
        if (FD_ISSET(fd2, &read_fds)) {
            update_sum(fd2, &sum);
        }
    }

    printf("%d\n", sum);

    return 0;
}
