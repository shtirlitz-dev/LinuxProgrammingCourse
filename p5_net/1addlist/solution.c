#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void prinAddresses(char ** items, int af)
{
    // Loop through the list of addresses in h->h_addr_list
    while (*items) {
        char ip_address[INET_ADDRSTRLEN];
        inet_ntop(af, *items, ip_address, INET_ADDRSTRLEN);

        printf("%s\n", ip_address);
        items++;
    }    
}

int main(int argc, char** argv)
{
    if(argc != 2)
        return 1;

    struct hostent *h;
    h = gethostbyname(argv[1]);

    if(!h) {
        perror("gethostbyname");
    }

    prinAddresses(h->h_addr_list, h->h_addrtype);
    return 0;
}
