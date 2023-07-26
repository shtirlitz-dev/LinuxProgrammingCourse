#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define USE_KEYS

int main(int argc, char ** argv)
{
    if(argc != 3)
        return 1;

    int shmid1 = atoi(argv[1]);
    int shmid2 = atoi(argv[2]);

#ifdef USE_KEYS
    // convert key to id
    shmid1 = shmget(shmid1, 1000, 0666);
    shmid2 = shmget(shmid2, 1000, 0666);
#endif

    // printf("%d %d\n", shmid1, shmid2);
    const int* addr1 = (int*)shmat(shmid1, NULL, SHM_RDONLY);
    const int* addr2 = (int*)shmat(shmid2, NULL, SHM_RDONLY);

#ifdef USE_KEYS
    int keyR = ftok(argv[0], 'R');
    int shmidRes = shmget(keyR, 1000, IPC_CREAT | IPC_EXCL | 0666);
#else
    int shmidRes = shmget(IPC_PRIVATE, 1000, IPC_CREAT | 0666);
#endif
    int* addrRes = (int*)shmat(shmidRes, NULL, 0);

    int i;
    for(i = 0; i < 100; ++i)
        addrRes[i] = addr1[i] + addr2[i];

    shmdt(addr1);
    shmdt(addr2);
    shmdt(addrRes);

#ifdef USE_KEYS
    printf("%d\n", keyR);
#else
    printf("%d\n", shmidRes);
#endif
 
    return 0;
}
