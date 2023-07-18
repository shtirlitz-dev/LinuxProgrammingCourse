#include <stdio.h>
#include <dirent.h>

int main(int argc, char ** argv)
{
    const char * name = ".";
    if(argc == 2)
        name = argv[1];
        
    DIR * dir = opendir(name);

    struct dirent *entry;
    while((entry = readdir(dir)) != 0) {
        printf("file: %d %s\n", entry->d_type, entry->d_name);
    }
    closedir(dir);
}