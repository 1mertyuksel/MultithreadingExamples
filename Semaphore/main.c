#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <semaphore.h> 

#define THREAD_NUMBER   10 

sem_t example_sem;

void *worker_thread(void *x) 
{
    printf("ENTRY for thread %d\n",*(int*)x);
    sem_wait(&example_sem);
    sleep(1);
    printf("for thread %d\n",*(int*)x);
    sleep(4);
    sem_post(&example_sem);
    free(x);
    printf("**********************************\n");

}


int main()
{
    pthread_t threads[THREAD_NUMBER];

    if(sem_init(&example_sem,0,4) !=0)
    printf("sem_init error");

    for(int i=0; i<THREAD_NUMBER; ++i)
    {
        int *ptr = (int*)malloc(sizeof(int));
        *ptr = i;
        if(pthread_create(&threads[i],NULL,worker_thread,ptr)!=0)
        printf("pthread_create error!!!\n");
    }


    for(int i=0; i<THREAD_NUMBER; ++i)
    {
        if(pthread_join(threads[i],NULL)!=0)
        {
            printf("pthread_join error\n");
        }
    }



}