#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int a;
    int b;
} numbers;

void* compute_sum(void* arg) {
    numbers* nums = (numbers*)arg;

    int* result = malloc(sizeof(int));
    *result = nums->a + nums->b;

    pthread_exit((void*)result);
}

int main() {
    pthread_t t1;
    numbers nums;

    nums.a = 10;
    nums.b = 20;

    pthread_create(&t1, NULL, compute_sum, &nums);

    int* sum;
    pthread_join(t1, (void**)&sum);

    printf("Sum = %d\n", *sum);

    free(sum);
    return 0;
}