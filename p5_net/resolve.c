#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void printList(char ** items)
{
    while(*items) {
        printf("'%s' ", *items);
        items++;
    }
    printf("\n");
}

void prinAddresses(char ** items, int af)
{
    // Loop through the list of addresses in h->h_addr_list
    while (*items) {
        // Extract the binary address from h->h_addr_list[i]
        //struct in_addr addr;
        //memcpy(&addr, *items, sizeof(struct in_addr));

        if(0) {
            // from course
            struct in_addr *addr = (struct in_addr *) *items;
            printf("'%s' ", inet_ntoa(*addr));
        }
        else {
            char ip_address[INET_ADDRSTRLEN];
            inet_ntop(af, *items, ip_address, INET_ADDRSTRLEN);

            printf("'%s' ", ip_address);
        }
        items++;
    }    
    printf("\n");
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

    printf("Canonical name: %s\n", h->h_name);
    printf("Aliases       : "); printList(h->h_aliases);
    printf("Type          : %s\n", h->h_addrtype == AF_INET? "ipv4" : 
                                   h->h_addrtype == AF_INET6? "ipv6" : "invalid");
    printf("Length        : %d\n", h->h_length);
    printf("Address list  : "); prinAddresses(h->h_addr_list, h->h_addrtype);
    return 0;
}


