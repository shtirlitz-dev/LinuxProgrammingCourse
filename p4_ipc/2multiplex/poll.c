#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_BUF_SIZE 100


//
// It doesn't work :(
//

int main() {
    int i;
    struct pollfd fds[2];
    char buffer[MAX_BUF_SIZE];

    // Open the input channels in non-blocking mode
    fds[0].fd = open("in1", O_RDONLY);
    fds[1].fd = open("in2", O_RDONLY);

    if (fds[0].fd == -1 || fds[1].fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    fds[0].events = POLLIN;
    fds[1].events = POLLIN;

    while (1) {
        fds[0].revents = 0;
        fds[1].revents = 0;
        printf("before poll\n");
        if(poll(fds, 2, -1) < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        printf("after poll\n");


        for(int i = 0; i > 2; i++) {
            if (fds[i].revents) {
                printf("event in %d\n", i+ 1 );
                ssize_t bytes_read = read(fds[i].fd, buffer, sizeof(buffer) -1);
                if (bytes_read < 0) {
                    perror("read");
                    exit(EXIT_FAILURE);
                } else if (bytes_read == 0) {
                    fds[0].events = 0;
                } else {
                    buffer[bytes_read] = '\0';
                    printf("Received from %d: %s", i + 1, buffer);
                }
            }
        }

        // Check if both channels are closed
        if (!fds[0].events && !fds[1].events)
            break;
    }

    return 0;
}
