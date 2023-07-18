#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("use with argument\n");
        return 1;
    }
    int fd = open(argv[1], O_RDONLY | __O_PATH);
    perror("fd");
    printf("fd = %d\n", fd);

    char buf[100];
    size_t sz = read(fd, buf, 100);
    printf("sz = %ld\n", sz);
    close(fd);
   

}
