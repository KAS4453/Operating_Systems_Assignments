#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

long long factorial(int x){
    long long fact = 1;
    for(int i = 1 ; i <= x ; i++){
        fact *= i;
    }
    return fact;
}

void fibonacci(int m){
    long long a = 0, b  = 1, next;

    printf("Fibonacci numbers for %d numbers: ", m);

    for(int i = 0 ; i < m ; i++){
        printf("%lld ", a);
        next = a + b;
        a = b;
        b = next;
    }
    printf("\n");
}

int main(){
    printf("This assignment is done by Kunwar Arpit Singh 22185.\n");
    pid_t pid;
    
    int n, m;
    
    printf("Enter the value of n (factorial count for parent).\n");
    scanf("%d",&n);

    printf("Enter the value of m (fibonacci count of child).\n");
    scanf("%d", &m);

    pid = fork();

    if(pid < 0){
        printf("Fork failed for this PID.\n");
        return 1;
    }
    else if(pid == 0){
        sleep(2);
        printf("Hi! I am a child process.\n");
        printf("My PID is %d.\n", getpid());
        printf("My Parent PID is %d.\n", getppid());
        fibonacci(m); 
    }
    else{
        printf("Hi! I am a parent process.\n");
        printf("My PID is %d.\n", getpid());
        printf("My Child PID is %d.\n", pid);

        long long result = factorial(n);
        printf("Factorial for number %d: %lld\n", n, result);
        printf("I am terminated earlier than my child.\n");
        exit(0);
    }
    return 0;
}