#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv)
{
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    printf("socket=%d\n", s);

    struct sockaddr_in local;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(1234);
    local.sin_family = AF_INET;

    int result = bind(s, (struct sockaddr*)&local,sizeof(local));
    printf("result=%d\n", result);

    char buf[BUFSIZ];
    int rd = read(s, buf, BUFSIZ - 1);
    buf[rd] = 0;
    printf("%s, bye\n", buf);
    close(s);
}
