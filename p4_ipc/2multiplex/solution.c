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

void my_sig_handler(int signo)
{
    printf("Called with %d\n", signo);
    //abort(); // to terminate
}

int main(int argc, char **argv) {
    signal(SIGPIPE, my_sig_handler);

    int fd1 = open("in1", O_RDONLY);
    int fd2 = open("in2", O_RDONLY);
    if (fd1 < 0 || fd2 < 0) 
        return EXIT_FAILURE;

    int sum = 0;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(fd1, &read_fds);
    FD_SET(fd2, &read_fds);
    int max_fd = (fd1 > fd2) ? fd1 : fd2;
    while(1)
    {
        fd_set input = read_fds;

        int ready_fds = select(max_fd + 1, &input, NULL, NULL, NULL);

        if (FD_ISSET(fd1, &input)) {
            if(!update_sum(fd1, &sum)) {
                FD_CLR(fd1, &read_fds);
            }
        }
        if (FD_ISSET(fd2, &input)) {
            if(!update_sum(fd2, &sum)) {
                FD_CLR(fd2, &read_fds);
            }
        }
        if(!FD_ISSET(fd1, &read_fds) && !FD_ISSET(fd2, &read_fds))
            break;
    }

    printf("%d\n", sum);

    return 0;
}
