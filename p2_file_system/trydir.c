#include <stdio.h>
#include <dirent.h>

int main()
{
    DIR * dir = opendir(".");
    struct dirent *entry;
    while((entry = readdir(dir)) != 0) {
        printf("file: %d %s\n", entry->d_type, entry->d_name);
    }
    closedir(dir);
}