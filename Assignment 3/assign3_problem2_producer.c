#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define SIZE 100

int main() {

    key_t key = 5678;

    int shmid = shmget(key, SIZE * sizeof(int), 0666 | IPC_CREAT);

    int *numbers = (int*) shmat(shmid, NULL, 0);

    srand(time(NULL));

    printf("Generated Numbers:\n");

    for(int i=0;i<SIZE;i++)
    {
        numbers[i] = rand() % 100;
        printf("%d ", numbers[i]);
    }

    printf("\nNumbers written to shared memory\n");

    shmdt(numbers);

    return 0;
}