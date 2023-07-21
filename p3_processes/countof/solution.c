#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

bool isPName(int pid, const char* pname)
{
    char filename[200];
    sprintf(filename, "/proc/%d/stat", pid);
    int f = open(filename, O_RDONLY);
    if(f == -1)
        return false;
    char bfr[201];
    size_t r = read(f, bfr, 200);
    bfr[r] = 0;
    close(f);
    return strstr(bfr, pname) != 0;
}

int main()
{
    DIR * dir = opendir("/proc");
    struct dirent *entry;
    int count = 0;
    while((entry = readdir(dir)) != 0) {
        char *e;
        int pid = (int)strtol(entry->d_name, &e, 10);
        if(*e || !pid)
            continue;
        if(!isPName(pid, "(genenv)"))
            continue;
        ++count;
    }
    closedir(dir);
    printf("%d\n", count);
}
