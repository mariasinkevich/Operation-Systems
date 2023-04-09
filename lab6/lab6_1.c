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
    printf("Writer has started\n");
    struct netent res_buf;
    struct netent *res;
    int h_errnop;
    struct pollfd pfd;
    errno = 0;
    args w;

    w.semr = sem_open("sem_read", O_CREAT, 0644, 0);
    if (w.semr == SEM_FAILED) printf("sem_open completed incorrectly with error: %s\n", strerror(errno));

    w.semw = sem_open("sem_write", O_CREAT, 0644, 0);
    if (w.semw == SEM_FAILED) printf("sem_open completed incorrectly with error: %s\n", strerror(errno));
    
    int fd = shm_open("memory", O_CREAT|O_RDWR, 0644);
    if (fd == -1) printf("shm_open completed incorrectly with error: %s\n", strerror(errno));

    int ret = ftruncate(fd, sizeof(char*));
    if (ret == -1) printf("ftruncate completed incorrectly with error: %s\n", strerror(errno));

    w.addr = (char*)mmap(0, sizeof(char*), PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if (w.addr == MAP_FAILED) printf("mmap completed incorrectly with error: %s\n", strerror(errno));

    char buf[1024];
    memset(buf, 0, 1024);
    int getret = getnetent_r(&res_buf, buf, 1024, &res, &h_errnop); 
    if (getret != 0) {
        printf("getnetent_r completed incorrectly with error: %s\n", strerror(h_errno)); 
        printf("Error status: %d; %d; %d\n", getret, ERANGE, ENOENT); }

    printf("Program is waiting for keystroke, please, press any key\n");
    while (1) {
        strcpy(buf, res_buf.n_name);
        printf("Writer result: %s\n", buf);

        memcpy(w.addr, &buf, 1024);

        sem_post(w.semw);
        sem_wait(w.semr);
        sleep(1);
        
        pfd.fd = STDIN_FILENO;
        pfd.events = POLLIN;
        pfd.revents = 0;
        struct timespec time = {0, 1};
        int ret = ppoll(&pfd, 1, &time, NULL);
        if (ret != 0) {
            printf("Key is pressed, thank you!\n");
            break;} }

    printf("Writer has finished\n");

    ret = munmap(0, sizeof(char*));
    if (ret == -1) printf("munmap completed incorrectly with error: %s\n", strerror(errno));

    close(fd);
    sem_close(w.semr);
    sem_close(w.semw);
    shm_unlink("memory");
    sem_unlink("sem_read");
    sem_unlink("sem_write");
    
return 0;
}







   
