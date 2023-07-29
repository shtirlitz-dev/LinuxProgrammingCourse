#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    if(argc != 2)
        return 1;
    int s = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in local;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(atoi(argv[1]));
    local.sin_family = AF_INET;

    int result = bind(s, (struct sockaddr*)&local,sizeof(local));
    if(result) {
        perror("bind");
        return 1;
    }

    while(1) {
        char buf[BUFSIZ];
        int rd = read(s, buf, BUFSIZ - 1);
        buf[rd] = 0;
        if(strncmp(buf, "OFF\n", 4) == 0)
            break;
        printf("%s\n", buf);
    }
    close(s);
}
