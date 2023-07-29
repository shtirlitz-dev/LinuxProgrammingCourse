#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

static int cmpchars(const void *p1, const void *p2)
{
    char ch1 = *((char*)p1);
    char ch2 = *((char*)p2);
    return ch1 > ch2 ? -1 : ch1 < ch2 ? 1 : 0;
}

void sortDesc(char *bfr, int size)
{
    qsort(bfr, size, 1, cmpchars);
}

int main(int argc, char** argv)
{
    if(argc != 2)
        return 1;
    int port = atoi(argv[1]);


    int ss = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    bind(ss, (struct sockaddr*)&local, sizeof(local));
    listen(ss, 5);

    // waiting for client
    int sc = accept(ss, NULL, NULL);
    while(1) {
        char buf[BUFSIZ];
        int rd = read(sc, buf, BUFSIZ - 1);
        buf[rd] = 0;
        if(strcmp(buf, "OFF") == 0)
            break;
        sortDesc(buf, rd);
        write(sc, buf, rd);
    }    
    close(sc);
    close(ss);
    return 0;
}
