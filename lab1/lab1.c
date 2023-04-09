#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

typedef struct {
    int flag;
    char sym;
} targs;

pthread_t t1;

void *proc1(void *arg) {
    if (pthread_equal(t1, pthread_self()) != 0) {
        printf("Equal identificators\n"); }
    else printf("Not equal identificators\n");
    targs *a1 = (targs *)arg;
    printf("Thread 1 has started!\n");
    while (a1->flag == 0)
    {
        putchar(a1->sym);
        fflush(stdout);
        sleep(1);
    }
    printf("Thread 1 has finished!\n");
    pthread_exit((void *)2);
}

void *proc2(void *arg) {
    targs *a2 = (targs *)arg;
    printf("Thread 2 has started!\n");
    while (a2->flag == 0) {
        putchar(a2->sym);
        fflush(stdout);
        sleep(1);
    }
    printf("Thread 2 has finished!\n");
    pthread_exit((void *)2);
}

int main() {
    printf("Program has started!\n");
    targs a1;
    targs a2;
    a1.flag = 0;
    a1.sym = '1';
    a2.flag = 0;
    a2.sym = '2';

    pthread_t id2;

    int err1 = pthread_create(&t1, NULL, &proc1, &a1);
    if (err1 != 0) {
        printf("Can't create thread 1, program has completed with errors\n");
        return 1;
    }

    int err2 = pthread_create(&id2, NULL, &proc2, &a2);
    if (err2 != 0) {
        printf("Can't create thread 2, program has completed with errors\n");
        return 2;
    }

    printf("Program is waiting for keystroke, please, press any key\n");
    getchar();
    printf("Key is pressed, thank you!\n");

    a1.flag = 1;
    a2.flag = 1;

    int *exitcode1, *exitcode2;
    char *strerror(int errnum);

    int errnum1 = pthread_join(t1, (void **)&exitcode1);
    if (errnum1 == 0) {
        printf("pthread_join 1 has completed correctly\n");
    }
    else {
        printf("pthread_join 1 has completed with errors: %s\n", strerror(errnum1));
    }

    int errnum2 = pthread_join(id2, (void **)&exitcode2);
    if (errnum2 == 0) {
        printf("pthread_join 2 has completed correctly\n");
    }
    else {
        printf("pthread_join 2 has completed with errors: %s\n", strerror(errnum2));
    }

    printf("Program has completed correctly!\n");

    return 0;
} 

  
