#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

typedef struct {
    sem_t* semr;
    sem_t* semw;
    char *addr;      
} args;

int main() {
    printf("Reader has started\n");
    struct pollfd pfd;
    errno = 0;
    args r;
    char buf[1024];

    r.semr = sem_open("sem_read", O_CREAT, 0644, 0);
    if (r.semr == SEM_FAILED) printf("sem_open completed incorrectly with error: %s\n", strerror(errno));

    r.semw = sem_open("sem_write", O_CREAT, 0644, 0);
    if (r.semw == SEM_FAILED) printf("sem_open completed incorrectly with error: %s\n", strerror(errno));
    
    int fd = shm_open("memory", O_CREAT|O_RDWR, 0644);
    if (fd == -1) printf("shm_open completed incorrectly with error: %s\n", strerror(errno));

    int ret = ftruncate(fd, sizeof(char*));
    if (ret == -1) printf("ftruncate completed incorrectly with error: %s\n", strerror(errno));

    r.addr = (char*)mmap(0, sizeof(char*), PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if (r.addr == MAP_FAILED) printf("mmap completed incorrectly with error: %s\n", strerror(errno));

    printf("Program is waiting for keystroke, please, press any key\n");
    while (1) {
        memset(buf, 0, 1024);
        sem_wait(r.semw);
        memcpy(&buf, r.addr, 1024);
        printf("Reader result: %s\n", buf);
        sem_post(r.semr);
        
        pfd.fd = STDIN_FILENO;
        pfd.events = POLLIN;
        pfd.revents = 0;
        struct timespec time = {0, 1};
        int ret = ppoll(&pfd, 1, &time, NULL);
        if (ret != 0) {
            printf("Key is pressed, thank you!\n");
            break;} }

    printf("Reader has finished\n");

    ret = munmap(0, sizeof(char*));
    if (ret == -1) printf("munmap completed incorrectly with error: %s\n", strerror(errno));

    close(fd);
    sem_close(r.semr);
    sem_close(r.semw);
    shm_unlink("memory");
    sem_unlink("sem_read");
    sem_unlink("sem_write");
return 0;
}







   
