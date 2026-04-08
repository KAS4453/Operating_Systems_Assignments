#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun {
    int val;
};

void wait_sem(int semid) {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void signal_sem(int semid) {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    int *X = (int*) shmat(shmid, NULL, 0);
    *X = 0;

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    union semun arg;
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);

    pid_t c1 = fork();

    if (c1 == 0) {
        for (int i = 0; i < 10; i++) {
            wait_sem(semid);
            *X += 2;
            signal_sem(semid);
        }
        exit(0);
    }

    pid_t c2 = fork();

    if (c2 == 0) {
        for (int i = 0; i < 10; i++) {
            wait_sem(semid);
            *X -= 2;
            signal_sem(semid);
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    printf("Final value of X: %d\n", *X);

    shmdt(X);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}