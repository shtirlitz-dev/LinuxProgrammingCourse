#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char ** argv)
{
    int with_brackets = 0;
    char string[] = "Some quotation, doesn't matter";
    int length = 0;
    int auth = 0;
    int was_invalid = 0;

    opterr = 0;

    struct option opts[] = {
        // set flag
        {"brackets", no_argument, &with_brackets, 1},
        // options arg
        {"length", required_argument, 0, 'l'},
        // longname option
        {"authornameshouldbeprinted", no_argument, 0, 'a'},
        // end
        {0,0,0,0}

    };

    while(1) {
        int optindex = 0;
        int optchar = getopt_long(argc, argv, "l:aq", opts, &optindex);
        printf("optchar=%d, optindex=%d", optchar, optindex);
        switch(optchar) {
            case -1:
                printf(" [end]");
            case 0:
                printf(" [0]");
                break;
            case 'l':
                length = atoi(optarg); // extern
                printf(" [l], length=%d", length);
                break;
            case 'a':
                auth = 1;
                printf(" [a]");
                break;
            case 'q':
                printf(" [q]");
                break;
            case '?':
                was_invalid = 1;
                printf(" [?]");
                break;
            default:
                printf(" [other]");
                break;
        }
        printf("\n");
        if(optchar == -1)
            break;
    }

    if(was_invalid)
        printf("some invalid option detected\n");

    if(auth)
        printf("Me(c)\n");
    

    if(length && length < strlen(string))
        string[length] = 0;
    if(with_brackets)
        printf("[%s]\n", string);
    else
        printf("%s\n", string);
    printf("Done\n");
}

