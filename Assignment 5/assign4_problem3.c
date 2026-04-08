#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SIZE 20

union semun { int val; };

void sem_wait(int semid, int i) {
    struct sembuf sb = {i, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal(int semid, int i) {
    struct sembuf sb = {i, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    int m, n;
    printf("Enter m (producers) and n (consumers): ");
    scanf("%d %d", &m, &n);

    // Shared memory
    int shmid = shmget(IPC_PRIVATE, sizeof(int)*(SIZE+3), IPC_CREAT | 0666);
    int *data = (int*) shmat(shmid, NULL, 0);

    int *buffer = data;
    int *in = &data[SIZE];
    int *out = &data[SIZE+1];
    int *sum = &data[SIZE+2];

    *in = *out = *sum = 0;

    // Semaphores: mutex, empty, full
    int semid = semget(IPC_PRIVATE, 3, IPC_CREAT | 0666);

    semctl(semid, 0, SETVAL, (union semun){1});     // mutex
    semctl(semid, 1, SETVAL, (union semun){SIZE});  // empty
    semctl(semid, 2, SETVAL, (union semun){0});     // full

    // Producers
    for (int i = 0; i < m; i++) {
        if (fork() == 0) {
            for (int j = 1; j <= 100; j++) {
                sem_wait(semid, 1); // empty
                sem_wait(semid, 0); // mutex

                buffer[*in] = j;
                *in = (*in + 1) % SIZE;

                sem_signal(semid, 0);
                sem_signal(semid, 2); // full
            }
            exit(0);
        }
    }

    // Consumers
    for (int i = 0; i < n; i++) {
        if (fork() == 0) {
            while (1) {
                sem_wait(semid, 2); // full
                sem_wait(semid, 0); // mutex

                int item = buffer[*out];
                *out = (*out + 1) % SIZE;

                sem_signal(semid, 0);
                sem_signal(semid, 1); // empty

                if (item == -1) break;

                *sum += item;
            }
            exit(0);
        }
    }

    // Wait for all producers
    for (int i = 0; i < m; i++) wait(NULL);

    // Insert poison pills
    for (int i = 0; i < n; i++) {
        sem_wait(semid, 1);
        sem_wait(semid, 0);

        buffer[*in] = -1;
        *in = (*in + 1) % SIZE;

        sem_signal(semid, 0);
        sem_signal(semid, 2);
    }

    // Wait for all consumers
    for (int i = 0; i < n; i++) wait(NULL);

    printf("Final SUM: %d\n", *sum);

    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}