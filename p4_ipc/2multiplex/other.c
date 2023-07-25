
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>


void read_pipe(int fd, long *sum, bool *eof_flag) {
    char buff[100];
    int bytes = read(fd, buff, 100); // read from pipe into char buffer
    
    if (bytes == 0)
        *eof_flag = true; // if nothing to read set end of file flag
    
    buff[bytes] = 0; //null terminator

    char **next = NULL;
    long value = strtol(buff, next, 10); // string to long
    *sum = *sum + value; // increment sum
}

int main(int argc, char **argv) {

    int in1 = open("in1", O_RDONLY); // open in1 and in2 as RDONLY
    int in2 = open("in2", O_RDONLY);

    if (!(in1 && in2)) { // if cannot open pipes
        printf("Cannot open one or both named pipes");
        return 1;
    }

    fd_set read_set;

    bool eof1 = false; // end of file flag for pipe1
    bool eof2  = false; // end of file flag for pipe2
    long sum = 0;

    while (true) {
        FD_ZERO(&read_set);
        FD_SET(in1, &read_set);
        FD_SET(in2, &read_set);

        int result = select(in2+1, &read_set, NULL, NULL, NULL); // waiting for read

        if (result) {
            if (FD_ISSET(in1, &read_set)) read_pipe(in1, &sum, &eof1); // calling handle
            if (FD_ISSET(in2, &read_set)) read_pipe(in2, &sum, &eof2);
        }

        if (eof1 && eof2) { // if both pipes are closed
            printf("%ld\n", sum); // output result
            break;
        }
    }

    return 0;
}