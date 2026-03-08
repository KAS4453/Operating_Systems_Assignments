#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SIZE 100

int main() {

    key_t key = 5678;

    int shmid = shmget(key, SIZE*sizeof(int), 0666);

    int *numbers = (int*) shmat(shmid, NULL, 0);

    int sum = 0;

    for(int i=0;i<SIZE;i++)
        sum += numbers[i];

    printf("Cumulative Sum = %d\n", sum);

    shmdt(numbers);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}