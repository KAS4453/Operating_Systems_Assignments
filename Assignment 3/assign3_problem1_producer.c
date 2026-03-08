#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "shared.h"

int main() {

    key_t key = 1234;

    int shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);

    struct shared_data *data = (struct shared_data*) shmat(shmid, NULL, 0);

    printf("Enter an integer: ");
    scanf("%d", &data->number);

    printf("Enter a string: ");
    scanf("%s", data->text);

    printf("Data written to shared memory\n");

    shmdt(data);

    return 0;
}