// epoll
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENTS 2
#define MAX_BUF_SIZE 100

void addEvent(int epfd, char * filename ) {
    int fd;
    struct epoll_event event;
    if ((fd = open (filename, O_RDONLY | O_NONBLOCK) ) < 0) {
        perror("open");
        exit(1);
    }
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event)) {
        perror("epoll_ctl(ADD)");
        exit(1);
    }
}

int main() {
    int epfd, i;
    struct epoll_event event, events[MAX_EVENTS];
    char buffer[MAX_BUF_SIZE];
    int chCount = 2;
    int sum = 0;

    // Create the epoll instance
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    addEvent(epfd, "in1");
    addEvent(epfd, "in2");


    while (chCount > 0) {
        int num_events = epoll_wait(epfd, events, MAX_EVENTS, -1); // Wait indefinitely for an event

        if (num_events == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (i = 0; i < num_events; i++) {
            int fd = events[i].data.fd;

            ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
            if (bytes_read < 0) {
                // The channel is closed
                perror("read");
                exit(1);
            } else if(bytes_read == 0) {
                //printf("A channel is closed\n");
                if (epoll_ctl (epfd, EPOLL_CTL_DEL,
                        events[i].data.fd, &events[i])) {
                    perror("epoll_ctl (DEL)");
                return 1;
                }
                close(events[i].data.fd);
                --chCount;
            } else {
                buffer[bytes_read] = '\0';
                //printf("Received: %s\n", buffer);
                sum += atoi(buffer);
            }
        }
    }

    printf("%d\n", sum);
    close(epfd);
    return 0;
}
