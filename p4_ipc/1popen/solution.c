#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char **argv) {
    if(argc != 3)
        return 1;
    char command[200];
    sprintf(command, "%s \"%s\"\n", argv[1], argv[2]);

    
    FILE* f = popen(command, "r");
    if(!f)
        return 1;

    char outbfr[100];
    int found = 0;
    while(1)
    {
        int cnt = fread(outbfr, 1, sizeof(outbfr), f);
        int i;
        for(i = 0; i < cnt; ++i)
        {
            if(outbfr[i] == '0')
                found++;
        }
        if(cnt < sizeof(outbfr))
            break;
    }

    pclose(f);
    printf("%d\n", found);

    return 0;
}
