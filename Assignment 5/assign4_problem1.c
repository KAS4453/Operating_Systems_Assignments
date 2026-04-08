#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main() {
    int shmid;
    int *X;

    // Create shared memory
    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    X = (int*) shmat(shmid, NULL, 0);

    *X = 0;

    pid_t c1 = fork();

    if (c1 == 0) {
        // Child C1
        for (int i = 0; i < 10; i++) {
            int temp = *X;
            sleep(1); // force race
            temp += 2;
            *X = temp;
        }
        exit(0);
    }

    pid_t c2 = fork();

    if (c2 == 0) {
        // Child C2
        for (int i = 0; i < 10; i++) {
            int temp = *X;
            sleep(1); // force race
            temp -= 2;
            *X = temp;
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    printf("Final value of X: %d\n", *X);

    shmdt(X);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}