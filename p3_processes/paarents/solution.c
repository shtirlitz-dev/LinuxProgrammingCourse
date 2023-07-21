// find parent up to 1 of a given process id

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int getParent(int pid)
{
    char filename[200];
    sprintf(filename, "/proc/%d/status", pid);
    int f = open(filename, O_RDONLY);
    if(f <0 )
        return 0;
    char bfr[201];
    size_t r = read(f, bfr, 200);
    bfr[r] = 0;
    close(f);
    const char* ppidpos = strstr(bfr, "PPid:");
    if(!ppidpos)
        return 0;
    int ppid;
    int n = sscanf(ppidpos + 5, "%d", &ppid);
    
    return n ? ppid : 0;
}

int main(int argc, char ** argv)
{
    if(argc != 2)
        return 1;

    int pid = atoi(argv[1]);
    while(pid) 
    {
        printf("%d\n", pid);
        if(pid == 1)
            break;
        pid = getParent(pid);
    }

    return 0;
}
