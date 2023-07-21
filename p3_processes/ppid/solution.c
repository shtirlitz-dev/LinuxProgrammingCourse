#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main()
{
    pid_t pid = getpid();
    char filename[200];
    sprintf(filename, "/proc/%d/status", pid);
    int f = open(filename, O_RDONLY);
    char bfr[201];
    size_t r = read(f, bfr, 200);
    bfr[r] = 0;
    close(f);
    const char* ppidpos = strstr(bfr, "PPid:");
    int ppid;
    sscanf(ppidpos + 5, "%d", &ppid);
    printf("%d\n", ppid);
    return 0;
}