#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h> // usleep
#include <stdbool.h>

#define STACK_SIZE 10000
char child_stack[STACK_SIZE + 1];

int child(void* params)
{
    int c = 0;
    while(true)
    {
        usleep(500000);
        printf("child turn %d\n", c++);
    }
}


int main()
{
    int c = 0;
#ifdef __linux__
    int result = clone(child, child_stack + STACK_SIZE, 0, 0);
#elif defined( __APPLE__ )
    int result = fork();
    if(!result)
        return child(0);
#else
    #error "Unsupported platform: This code requires Linux or macOS."
#endif
    printf("clone result = %d\n", result);

    while(true)
    {
        usleep(500000);
        printf("parent turn %d\n", c++);
    }
    return 0;
}
