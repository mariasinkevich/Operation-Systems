#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <malloc.h>

int main() {
    int n;
    printf ("Enter the number of attributes: ");
    scanf ("%d", &n);
    int num = n+1;

    char **argv;
    argv = (char**)malloc(num * sizeof(char*));
    for (int i = 0; i < n; i++) {
        argv[i] = (char*)malloc(20 * sizeof(char));
        printf("Enter attribute в„–%d: ", i+1);
        scanf("%s", argv[i]); }
    argv[n] = NULL;
    
    setenv("PATH", ".", 1);
    extern char **environ;

    int status;
    
    pid_t pid =	fork();
    if (pid == 0) {
        int ex = execvpe("lab3_1", argv, __environ); 
        if (ex == -1) {
            printf("execvpe completed incorrectly");
            perror("execvpe");
            exit(1); } }
    else if (pid == -1) {
        printf("fork completed incorrectly");
        perror ("fork");
        exit(1); }
    else {
        printf("Parent process has started!\n");
        do {
            pid = waitpid(pid, &status, WNOHANG);
            if (pid == 0) {
                printf("Please, wait\n");
                sleep(0.5); }
            else if (pid == -1) {
                printf("waitpid completed incorrectly\n");
                perror ("waitpid"); 
                exit(1); }
            else {
                printf ("Current pid: %d \n", getpid());
                printf ("Child pid: %d \n", pid);
                printf ("Parent pid: %d \n", getppid());
                printf("Child process ended with the code: %d \n", status); } 
        } while (pid == 0);
        printf("Parent process has finished!\n");
    }
    for (int i = 0; i < num; i++) {
        free(argv[i]);
    }
    free(argv);
    return 0;
}









