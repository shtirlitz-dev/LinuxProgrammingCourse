// find parent up to 1 of a given process id

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

typedef struct Set_
{
    int *bfr;
    int count;
    int size;
} Set;

void initSet(Set* s)
{
    s->size = 10000;
    s->bfr = (int*) malloc(s->size * sizeof(int*));
    s->count = 0;
}
void deleteSet(Set* s)
{
    free(s->bfr);
}

bool hasSet(Set*s, int n)
{
    for(int i = 0; i < s->count; ++i)
    {
        if(s->bfr[i] == n)
            return true;
    }
    return false;
}

void addSet(Set*s, int n)
{
    if(s->count == s->size) {
        s->size += 10000;
        s->bfr = (int*) realloc(s->bfr, s->size * sizeof(int*));
    }
    s->bfr[s->count++] = n;
}

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

Set children, notChildren;

bool evaluate(int sppid, int pid)
{
    if(pid == 1 || pid == 0)
        return false;

    if(hasSet(&children, pid))
        return true;

    if(hasSet(&notChildren, pid))
        return false;

    int ppid = getParent(pid);
    bool isChild = pid == sppid || ppid == sppid || evaluate(sppid, ppid);
   // printf("%d %d: %d\n", pid, ppid, isChild);
    addSet(isChild? &children : &notChildren, pid);
    return isChild;
}

int getChildrenCount(int ppid)
{
    initSet(&children);
    initSet(&notChildren);

    DIR * dir = opendir("/proc");
    struct dirent *entry;
    while((entry = readdir(dir)) != 0) {
        char *e;
        int pid = (int)strtol(entry->d_name, &e, 10);
      //  printf("/proc/%s type %d\n", entry->d_name, entry->d_type);
        if(*e || !pid || pid == 1)
            continue;
        evaluate(ppid, pid);
    }
    closedir(dir);
    deleteSet(&children);
    deleteSet(&notChildren);
    return children.count;
}

int main(int argc, char ** argv)
{
    if(argc != 2)
        return 1; //getChildrenCount(578);
/*
    Set a;
    initSet(&a);
    addSet(&a, 1);
    addSet(&a, 3);
    addSet(&a, 7);
    addSet(&a, 5);

    printf("%d\n", hasSet(&a, 4));
    printf("%d\n", hasSet(&a, 5));
    printf("%d\n", hasSet(&a, 6));

    deleteSet(&a);
*/
    int pid = atoi(argv[1]);
    printf("%d\n", getChildrenCount(pid));

    return 0;
}
