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
#ifdef USE_KEYS
    int key1 = ftok(argv[0], 'R');
    int key2 = ftok(argv[0], 'R');
    int shmid1 = shmget(key1, 1000, IPC_CREAT | 0666);
    int shmid2 = shmget(key2, 1000, IPC_CREAT | 0666);
    printf("%d %d\n", key1, key2);
#else
    int shmid1 = shmget(IPC_PRIVATE, 1000, IPC_CREAT | 0666);
    int shmid2 = shmget(IPC_PRIVATE, 1000, IPC_CREAT | 0666);
    printf("%d %d\n", shmid1, shmid2);
#endif


    int* addr1 = (int*)shmat(shmid1, NULL, 0);
    int* addr2 = (int*)shmat(shmid2, NULL, 0);

//    int* addrRes = (int*)shmat(shmidRes, NULL, 0);

    int i;
    for(i = 0; i < 100; ++i) {
        addr1[i] = i;
        addr2[i] = i + 1;
    }

    shmdt(addr1);
    shmdt(addr2);
//    shmdt(addrRes);

    printf("done, enter a digit to exit: ");
    
    int shmidR;
    scanf("%d", &shmidR);
    
#ifdef USE_KEYS
    // convert key to id
    shmidR = shmget(shmidR, 1000, 0666);
#endif

    int* addrR = (int*)shmat(shmidR, NULL, 0);
    for(i = 0; i < 100; ++i) {
    	printf("%d ", addrR[i]);
    }
    printf("\n");
    shmdt(addrR);
    
    
    shmctl(shmid1, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    shmctl(shmidR, IPC_RMID, NULL);
    
    
    
 
    return 0;
}
