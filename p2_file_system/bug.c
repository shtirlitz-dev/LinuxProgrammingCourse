#include <stdio.h>

int* f()
{
    return (int*) 42;
}

void print(int *p)
{
    printf("We use %d\n", *p);
}

int main()
{
    print(f());
}
