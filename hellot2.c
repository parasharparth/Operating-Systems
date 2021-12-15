#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXTHREADS 50
#define SLEEPTIME 5
volatile unsigned long long Total = 0ULL;
pthread_mutex_t mVar=PTHREAD_MUTEX_INITIALIZER;
void *Factorial(void *tidptr)
{
        sleep(SLEEPTIME);
        int tid = * ((int*) tidptr);
        //printf("the value of tid is:- %d\n",tid);
        if (tid > 20) {
                fprintf(stderr, "ERROR: thread %d exit to avoid long long overflow\n", tid);
                return NULL;
        }
        unsigned long long factorial = 1;
        for (int j = 1; j <= tid; j++) {
                pthread_mutex_lock(&mVar);
                factorial *= j;
                pthread_mutex_unlock(&mVar);
        }
        printf("Thread %d result is %llu\n", tid, factorial);
        return NULL;
}
int main(int argc, char *argv[])
{
        if (argc != 2) {
                fprintf(stderr, "USAGE: helloT <nthreads>\n");
                exit(-1);
        }
        int nthreads = atoi(argv[1]);
        if (nthreads < 1) {
                fprintf(stderr, "ERROR: numthreads must be >= 1\n");
                exit(-1);
        }
        printf("User requested %d threads\n", nthreads);
        int rc;
        pthread_t t[nthreads];
        int tids[MAXTHREADS];
        for (int i = 0; i < nthreads; i++) {
                tids[i] = i+1;
                rc = pthread_create(&t[i], NULL, Factorial, (void *) &tids[i]);
                if (rc){
                        printf("ERROR; return code from pthread_create() is %d\n", rc);
                        exit(-1);
                }
        }
        for (int i = 0; i < nthreads; i++) {
                rc = pthread_join(t[i], NULL);
                if (rc != 0) {
                        fprintf(stderr, "ERROR joining with thread %d (error==%d)\n", tids[i], rc);
                        exit(-1);
                }
        }
        exit(0);
}