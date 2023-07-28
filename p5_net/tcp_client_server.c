#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>


void client();
void server();

int main(int argc, char** argv)
{
    char type = argc == 2? *argv[1] : 0;
    if(type == 'c')
        client();
    else if (type == 's')
        server();
    else
        printf("Use %s [s|c]\n", argv[0]);
    return 0;
}

void client()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(1234);
    local.sin_family = AF_INET;

    connect(s, (struct sockaddr*)&local, sizeof(local));

    char buf[BUFSIZ] = "Hello\n";
    write(s, buf, strlen(buf + 1));
    close(s);
}

void server()
{
    int ss = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in local;
    inet_aton("127.0.0.1", &local.sin_addr);
    local.sin_port = htons(1234);
    local.sin_family = AF_INET;

    bind(ss, (struct sockaddr*)&local, sizeof(local));
    listen(ss, 5);

    // waiting for client
    int sc = accept(ss, NULL, NULL);

    char buf[BUFSIZ];
    int rd = read(sc, buf, BUFSIZ - 1);
    buf[rd] = 0;
    printf("%s\n", buf);
    close(sc);
    close(ss);

}
