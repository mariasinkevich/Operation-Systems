#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int flag;
    char sym;
} targs;

pthread_mutex_t mutex;

void *proc1(void *arg) {
    targs *a1 = (targs *)arg;
    char *strerror(int errnum);
    printf("Thread 1 has started!\n");
    while (a1->flag == 0){
        int errlock1 = pthread_mutex_lock(&mutex);
        if (errlock1 == 0){
            printf("\npthread_mutex_lock 1 has completed correctly\n"); }
        else {
            printf("pthread_mutex_lock 1 has completed with errors: %s\n", strerror(errlock1));}
        for (int i = 0; i < 10; i++){
            putchar(a1->sym);
            fflush(stdout);
            sleep(1); }
        int errunlock1 = pthread_mutex_unlock(&mutex);
        if (errunlock1 == 0){
            printf("\npthread_mutex_unlock 1 has completed correctly\n"); }
        else {
            printf("\npthread_mutex_unlock 1 has completed with errors: %s\n", strerror(errunlock1));}
        sleep(1);
    }
    printf("Thread 1 has finished!\n");
    pthread_exit((void *)2);
}

void *proc2(void *arg) {
    targs *a2 = (targs *)arg;
    char *strerror(int errnum);
    printf("Thread 2 has started!\n");
    while (a2->flag == 0){
        int errlock2 = pthread_mutex_lock(&mutex);
        if (errlock2 == 0){
            printf("\npthread_mutex_lock 2 has completed correctly\n"); }
        else {
            printf("\npthread_mutex_lock 2 has completed with errors: %s\n", strerror(errlock2));}
        for (int i = 0; i < 10; i++){
            putchar(a2->sym);
            fflush(stdout);
            sleep(1); }
        int errunlock2 = pthread_mutex_unlock(&mutex);
        if (errunlock2 == 0){
            printf("\npthread_mutex_unlock 2 has completed correctly\n"); }
        else {
            printf("\npthread_mutex_unlock 2 has completed with errors: %s\n", strerror(errunlock2));}
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

    pthread_t id1, id2;

    int errinit = pthread_mutex_init(&mutex, NULL); 
    if (errinit == 0){
            printf("\npthread_mutex_init has completed correctly\n\n"); }
        else {
            printf("\npthread_mutex_init has completed with errors: %s\n", strerror(errinit));}

    int err1 = pthread_create(&id1, NULL, &proc1, &a1);
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

    int errnum1 = pthread_join(id1, (void **)&exitcode1);
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

    int errdest = pthread_mutex_destroy(&mutex);
    if (errdest == 0){
            printf("\npthread_mutex_destroy has completed correctly\n\n"); }
        else {
            printf("\npthread_mutex_destroy has completed with errors: %s\n", strerror(errdest));}
    
    printf("Program has completed correctly!\n");

    return 0;
} 




