#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();

    printf("This assignment is done by Kunwar Arpit Singh 22185.\n");

    if (pid < 0){
        printf("Forking failed for this PID.\n");
        return 1;
    }

    else if (pid ==0){
        printf("Hi! I am a child process.\n");
        printf("My PID is %d.\n", getpid());
        printf("My Parent PID is %d.\n", getppid());
    }

    else{
        printf("Hi! I am a parent process.\n");
        printf("My PID is %d.\n", getpid());
        printf("My Child PID is %d.\n", pid);
        wait(NULL);
    }

    return 0;
}