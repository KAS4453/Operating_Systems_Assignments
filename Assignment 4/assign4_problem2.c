#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

int fd;

void* create_file(void* arg) {
    fd = open("data.txt", O_CREAT | O_WRONLY, 0644);
    if (fd < 0) {
        perror("File creation failed");
    } else {
        printf("File created successfully\n");
    }
    return NULL;
}

void* write_file(void* arg) {
    sleep(1); // ensure file is created first

    char data[] = "Hello, this is multithreading assignment.\n";

    write(fd, data, sizeof(data));
    printf("Data written to file\n");

    close(fd);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, create_file, NULL);
    pthread_create(&t2, NULL, write_file, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}