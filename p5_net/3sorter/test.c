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
    int port = atoi(argv[1]);

    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(port);
    local.sin_family = AF_INET;

    connect(s, (struct sockaddr*)&local, sizeof(local));

    const char* strings[] = {
        "Hello",
        "ABCDEF",
        "xyz\n",
        "OFF",
        0
    };
   
    for(int i = 0; strings[i]; ++i) {
        sleep(1);
        const char* str = strings[i];        
        write(s, str, strlen(str) + 1);
        if(strcmp(str, "OFF") == 0)
            break;

        char buf[BUFSIZ];
        int rd = read(s, buf, BUFSIZ - 1);
        buf[rd] = 0;
        printf("received: [%s]\n", buf);
    }
    close(s);
    return 0;
}
