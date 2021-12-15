#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXTHREADS 50
#define SLEEPTIME 5
void *Factorial(void *tidptr)
{
        sleep(SLEEPTIME);
        int tid = * ((int*) tidptr);
        printf("the value of tid is %d\n",tid);
        if (tid > 20) {
                fprintf(stderr, "ERROR: thread %d exit to avoid long long overflow\n", tid);
                return NULL;
        }
        unsigned long long factorial = 1;
        for (int j = 1; j <= tid; j++) {
                factorial *= j;
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
        pthread_t t;
        int i = 0;
        for(i = 1; i <= nthreads; i++)
        {
                rc = pthread_create(&t, NULL, Factorial, (void *) &i);
                if (rc){
                        fprintf(stderr, "ERROR; return code from pthread_create() is %d\n", rc);
                        exit(-1);
                }
                rc = pthread_join(t, NULL);
        }

        exit(0);
}