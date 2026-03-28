#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define SIZE 10000

int arr[SIZE];

void* process_array(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        arr[i] = (arr[i] + 2) * 4;

        printf("%d ", arr[i]);

        pthread_testcancel(); // allow cancellation
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // Initialize array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }

    pthread_create(&t1, NULL, process_array, NULL);
    pthread_create(&t2, NULL, process_array, NULL);

    sleep(1);

    pthread_cancel(t1);
    pthread_cancel(t2);

    printf("\nThreads cancelled after 1 second\n");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}