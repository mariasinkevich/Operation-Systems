#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[], char *envp[]) {
    printf ("Child process has started!\n");
    for (int i = 0; i < argc; i++) {
        printf ("%s \n", argv[i]);
        sleep(1); }
    
    printf ("Child pid: %d \n", getpid());
    printf ("Parent pid: %d \n", getppid());

    printf ("Child process has finished!\n");
    
    exit(0);
}
