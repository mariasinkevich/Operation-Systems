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
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct {
    long mtype;
    char buf[1024];
}tmessage;

int main() {
    printf("Writer has started\n");
    
    struct netent res_buf;
    struct netent *res;
    int h_errnop;
    
    struct pollfd pfd;
    errno = 0;
    
    tmessage m;

    m.mtype = 1;

    FILE *file = fopen("file.txt", "a+");
    key_t mkey = ftok("file.txt",'A');
    if (mkey == -1) printf("ftok completed incorrectly with error: %s\n", strerror(errno));

    int msgid = msgget(mkey, 0);
    if (msgid < 0) {
        msgid = msgget(mkey, 0666 | IPC_CREAT); }

    memset(m.buf, 0, 1024);
    int getret = getnetent_r(&res_buf, m.buf, 1024, &res, &h_errnop); 
    if (getret != 0) {
        printf("getnetent_r completed incorrectly with error: %s\n", strerror(h_errno)); 
        printf("Error status: %d; %d; %d\n", getret, ERANGE, ENOENT); }

    printf("Program is waiting for keystroke, please, press any key\n");
    while (1) {
        strcpy(m.buf, res_buf.n_name);
        printf("Writer result: %s\n", m.buf);

        int res = msgsnd(msgid, &m, 1024, IPC_NOWAIT); 
        //int res = msgsnd(msgid, &m, 1024, 0);
        if (res == -1) {
            printf("msgsnd completed incorrectly with error: %s\n", strerror(errno));
            break;}

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

    fclose(file);
    msgctl(msgid, IPC_RMID , NULL);
}







   
