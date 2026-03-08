#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared.h"

int main() {

    key_t key = 1234;

    int shmid = shmget(key, sizeof(struct shared_data), 0666);

    struct shared_data *data = (struct shared_data*) shmat(shmid, NULL, 0);

    printf("String from producer: %s\n", data->text);

    if(data->number % 2 == 0)
        printf("Number %d is EVEN\n", data->number);
    else
        printf("Number %d is ODD\n", data->number);

    shmdt(data);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}