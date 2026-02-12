#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    printf("This assignment is done by Kunwar Arpit Singh 22185.\n");

    printf("Hi! I am a root process.\n\n");
    printf("My PID is %d.\n", getpid());

    pid_t pidQ = fork();

    if(pidQ == 0){
        printf("Hi! I am a child process of Q of Parent  P.\n");
        printf("My PID is %d.\n", getpid());
        printf("My Parent PID is %d.\n", getppid());
        
        pid_t grandParentPID = getppid();
        pid_t pidS = fork();

        if(pidS == 0){
            printf("Hi! I am a child process S of Parent Q.\n");
            printf("My PID is %d.\n", getpid());
            printf("My Parent PID is %d.\n", getppid());
            printf("My Grand Parent PID is %d.\n", grandParentPID);
            exit(0);
        }

        pid_t pidT = fork();
        if(pidT == 0){
            printf("Hi! I am a child process T of Parent Q.\n");
            printf("My PID is %d.\n", getpid());
            printf("My Parent PID is %d.\n", getppid());
            printf("My Grand Parent PID is %d.\n", grandParentPID);
            exit(0);
        }      
        wait(NULL);
        wait(NULL);
        exit(0);
    }

    else{  
        pid_t pidR = fork();

        if(pidR == 0){
            printf("Hi! I am a child process R of Parent P.\n");
            printf("My PID is %d.\n", getpid());
            printf("My Parent PID is %d.\n", getppid());

            pid_t grandParentPID = getppid();
            pid_t pidU = fork();

            if(pidU == 0){
                printf("Hi! I am a child process U of Parent R.\n");
                printf("My PID is %d.\n", getpid());
                printf("My Parent PID is %d.\n", getppid());
                printf("My Grand Parent PID is %d.\n", grandParentPID);
                exit(0);
            }

        wait(NULL);
        exit(0);
        }

    }
    wait(NULL);
    wait(NULL);    
    return 0;
}