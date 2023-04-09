#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

typedef struct {
    int flag;
} flags;

int filedes[2];

void* writer(void *arg) {
    flags *w = (flags *)arg;
    printf("Writer has started\n"); 
    
    char buf[1024];
    errno = 0;

    struct netent res_buf;
    struct netent *res;
    int h_errnop;

    memset(buf, 0, 1024);
    int getret = getnetent_r(&res_buf, buf, 1024, &res, &h_errnop); 
    if (getret != 0) {
        printf("getnetent_r completed incorrectly with error: %s\n", strerror(h_errno)); 
        printf("Error status: %d; %d; %d\n",getret, ERANGE,ENOENT); }
    
    strcpy(buf, res_buf.n_name);

    while (w->flag == 0) {
        int writeret = write(filedes[1], &buf, 1024);
        if(writeret == -1) {
            printf("write completed incorrectly with error: %s\n", strerror(errno)); }
        sleep(1);
    }

    printf("Writer has finished\n");
    pthread_exit((void *)2);
}


void* reader(void *arg) {
    flags *r = (flags *)arg;
    printf("Reader has started\n\n");

    char buf[1024];
    errno = 0;

    while (r->flag == 0) {
        memset(buf, 0, 1024);

        int readret = read(filedes[0], &buf, 1024);
        if(readret != -1) {
            printf ("Read result: %s\n", buf); }
        else {
            printf("read completed incorrectly with error: %s\n", strerror(errno)); }
        sleep(1); 
    }

    printf("Reader has finished\n");
    pthread_exit((void *)2);
}


int main() {
    flags r, w;
    r.flag = 0;
    w.flag = 0;

    pthread_t idw, idr;
    
    pipe(filedes);
    //pipe2(filedes, O_NONBLOCK);
    //fcntl(filedes[1], F_SETFL, O_NONBLOCK);
    //fcntl(filedes[0], F_SETFL, O_NONBLOCK);

    pthread_create(&idw, NULL, &writer, &w);
    pthread_create(&idr, NULL, &reader, &r);

    printf("Program is waiting for keystroke, please, press any key\n");
    getchar();
    printf("Key is pressed, thank you!\n");
    
    r.flag = 1;
    w.flag = 1;
    
    pthread_join(idw, NULL);
    pthread_join(idr, NULL);

    close(filedes[0]);
    close(filedes[1]);

return 0;
}
