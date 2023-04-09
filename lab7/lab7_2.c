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
    printf("Reader has started\n");
    
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

    printf("Program is waiting for keystroke, please, press any key\n");
    while (1) {
        memset(m.buf, 0, 1024);
        ssize_t res = msgrcv(msgid, &m, 1024, m.mtype, IPC_NOWAIT);
        //ssize_t res = msgrcv(msgid, &m, 1024, m.mtype, 0);
        if (res != -1) {
            printf("Reader result: %s\n", m.buf); }
    
        pfd.fd = STDIN_FILENO;
        pfd.events = POLLIN;
        pfd.revents = 0;
        struct timespec time = {0, 1};
        int ret = ppoll(&pfd, 1, &time, NULL);
        if (ret != 0) {
            printf("Key is pressed, thank you!\n");
            break;} }

    printf("Reader has finished\n");

    fclose(file);
    msgctl(msgid, IPC_RMID , NULL);
}


