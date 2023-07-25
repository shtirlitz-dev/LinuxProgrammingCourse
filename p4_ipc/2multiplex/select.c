
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <string.h>

#define MAX_BUF_SIZE 100

int main() {
    int in1_fd, in2_fd;
    fd_set read_fds;
    char buffer[MAX_BUF_SIZE];

    // Open the input channels in non-blocking mode
    in1_fd = open("in1", O_RDONLY | O_NONBLOCK);
    in2_fd = open("in2", O_RDONLY | O_NONBLOCK);

    if (in1_fd == -1 || in2_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int max_fd = (in1_fd > in2_fd) ? in1_fd : in2_fd;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(in1_fd, &read_fds);
        FD_SET(in2_fd, &read_fds);

        // Wait for input on either channel
        int ready_fds = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (ready_fds == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Check if in1 has data to read
        if (FD_ISSET(in1_fd, &read_fds)) {
            ssize_t bytes_read = read(in1_fd, buffer, sizeof(buffer));
            if (bytes_read < 0) {
                // in1 channel is closed
                close(in1_fd);
                FD_CLR(in1_fd, &read_fds);
            } else {
                buffer[bytes_read] = '\0';
                int num = atoi(buffer);
                printf("Received number from in1: %d\n", num);
            }
        }

        // Check if in2 has data to read
        if (FD_ISSET(in2_fd, &read_fds)) {
            ssize_t bytes_read = read(in2_fd, buffer, sizeof(buffer));
            if (bytes_read < 0) {
                // in2 channel is closed
                close(in2_fd);
                FD_CLR(in2_fd, &read_fds);
            } else {
                buffer[bytes_read] = '\0';
                int num = atoi(buffer);
                printf("Received number from in2: %d\n", num);
            }
        }

        // Check if both channels are closed
        if (!FD_ISSET(in1_fd, &read_fds) && !FD_ISSET(in2_fd, &read_fds)) {
            break;
        }
    }

    return 0;
}


#if 0
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

int main(void) {
 int fds[2];
 char buf[4096];
 int i, rc, maxfd;
 fd_set watchset; /* fds для чтения */
 fd_set inset; /* обновляется select() */

 /* открыть оба канала */
 if ((fds[0] = open("p1", O_RDONLY | O_NONBLOCK)) < 0) {
  perror("open p1");
  return 1;
 }

 if ((fds[1] = open("p2", O_RDONLY | O_NONBLOCK)) < 0) {
  perror("open p2");
  return 1;
 }

 /* начать чтение из обоих файловых дескрипторов */
 FD_ZERO(&watchset);
 FD_SET(fds[0], &watchset);
 FD_SET(fds[1], &watchset);

 /* найти максимальный файловый дескриптор */
 maxfd = fds[0] > fds[1] ? fds[0] : fds[1];

 /* пока наблюдаем за одним из fds[0] или fds[1] */
 while (FD_ISSET(fds[0], &watchset) ||
  FD_ISSET(fds[1], &watchset)) {
  /* здесь копируем watchset, потому что select() обновляет его */
  inset = watchset;
  if (select(maxfd + 1, &inset, NULL, NULL, NULL) < 0) {
   perror("select");
   return 1;
  }

  /* проверить, какой из файловых дескрипторов
     готов для чтения из него */
  for (i = 0; i < 2; i++) {
   if (FD_ISSET(fds[i], &inset )) {
    /* fds[i] готов для чтения, двигаться дальше... */
    rc = read(fds[i], buf, sizeof (buf) - 1);
    if (rc < 0) {
     perror("read");
     return 1;
    } else if (!rc) {
     /* этот канал закрыт, не пытаться
        читать из него снова */
     close(fds[i]);
     FD_CLR(fds[i], &watchset);
    } else {
     buf[rc] = '\0';
     printf("чтение: %s", buf);
    }
   }
  }
 }

 return 0;
}
#endif