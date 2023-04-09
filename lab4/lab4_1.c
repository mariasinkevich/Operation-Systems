#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

sem_t* sem;

int main() {
    struct pollfd pfd;

    sem = sem_open("/sem", O_CREAT, 0644, 1);
    FILE *file = fopen("file.txt", "a+");

    while(1){
        int wait = sem_wait(sem);
        if(wait == 0) {
            for (int i = 0; i <= 10; i++) {
                printf("%c", '1');
                fflush(stdout);
                fputc('1', file);
                fflush(file);
                sleep(1); } 

            sem_post(sem);
            sleep(1);

            pfd.fd = STDIN_FILENO;
            pfd.events = POLLIN;
            pfd.revents = 0;
            struct timespec time = {0, 1};
            int ret = ppoll(&pfd, 1, &time, NULL);
            if (ret != 0) break; } }
    
    fclose(file);
    sem_close(sem);
    sem_unlink("/sem");

return 0;
}



