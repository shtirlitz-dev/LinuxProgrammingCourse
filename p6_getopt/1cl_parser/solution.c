#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char ** argv)
{
    int was_invalid = 0;
    opterr = 0;

    struct option opts[] = {
        {"query", required_argument, 0, 'q'},
        {"longinformationrequest", no_argument, 0, 'i'},
        {"version", no_argument, 0, 'v'},
        {0,0,0,0}
    };

    while(1) {
        int optindex = 0;
        int optchar = getopt_long(argc, argv, "q:iv", opts, &optindex);
        //printf("%d\n", optchar);
        if(optchar == -1)
            break;
        if(optchar == '?') {
            was_invalid = 1;
            break;
        }
    }

    printf("%c\n", was_invalid? '-' : '+');
    return 0;
}

