#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 10

long factorial(int n)
{
    long f = 1;
    for(int i = 1; i <= n; i++)
        f *= i;
    return f;
}

int main()
{
    key_t key = 5678;

    int shmid = shmget(key, SIZE * sizeof(int), 0666 | IPC_CREAT);

    int *arr = (int *)shmat(shmid, NULL, 0);

    printf("Enter 10 numbers:\n");

    for(int i = 0; i < SIZE; i++)
        scanf("%d", &arr[i]);

    /* Consumer 1 */
    if(fork() == 0)
    {
        printf("\nConsumer 1 (first 4 numbers)\n");
        for(int i = 0; i < 4; i++)
            printf("Factorial of %d = %ld\n", arr[i], factorial(arr[i]));
        exit(0);
    }

    /* Consumer 2 */
    if(fork() == 0)
    {
        printf("\nConsumer 2 (next 2 numbers)\n");
        for(int i = 4; i < 6; i++)
            printf("Factorial of %d = %ld\n", arr[i], factorial(arr[i]));
        exit(0);
    }

    /* Consumer 3 */
    if(fork() == 0)
    {
        printf("\nConsumer 3 (remaining numbers)\n");
        for(int i = 6; i < 10; i++)
            printf("Factorial of %d = %ld\n", arr[i], factorial(arr[i]));
        exit(0);
    }

    /* wait for children */
    for(int i = 0; i < 3; i++)
        wait(NULL);

    shmdt(arr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}