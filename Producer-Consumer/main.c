#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

typedef struct CircularQueue {
    int *pQueue;
    int front;
    int rear;
    size_t capacity;
    size_t size;
} CircularQueue, *HndCircularQueue;

HndCircularQueue initCircularQueue(size_t cap);
int enqueue(HndCircularQueue hnd, int value);
int dequeue(HndCircularQueue hnd, int *pVal);

size_t getSize(HndCircularQueue hnd);
size_t getCapacity(HndCircularQueue hnd);
int isEmpty(HndCircularQueue hnd);
void reset(HndCircularQueue hnd);
void destroy(HndCircularQueue hnd);

#define QSIZE           100
#define THREAD_NUMBER   2

sem_t producer_sem, consumer_sem;
int ival;

void *thread_producer(void *x) {
    HndCircularQueue handle = (HndCircularQueue)x;

    printf("thread producer is starting...\n");
    int i = 0;
    while (1) {
        sem_wait(&producer_sem);
        enqueue(handle, i);
        sem_post(&consumer_sem);
        ++i;
        if (i == 10)
            break;
    }
    printf("finishing producer thread.\n");

    return NULL;
}

void *thread_consumer(void *x) {
    HndCircularQueue handle = (HndCircularQueue)x;
    printf("thread consumer is starting...\n");

    while (1) {
        sem_wait(&consumer_sem);
        dequeue(handle, &ival);
        sem_post(&producer_sem);
        printf("%d\n", ival);
        sleep(1);
        if (ival == 9)
            break;
    }

    printf("finishing consumer thread.\n");
    return NULL;
}

int main() {
    HndCircularQueue queue = initCircularQueue(QSIZE);

    // Initialize semaphores
    sem_init(&producer_sem, 0, 1);
    sem_init(&consumer_sem, 0, 0);

    // Create producer and consumer threads
    pthread_t producer_thread, consumer_thread;

    if (pthread_create(&producer_thread, NULL, thread_producer, (void *)queue) != 0) {
        perror("Error creating producer thread");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&consumer_thread, NULL, thread_consumer, (void *)queue) != 0) {
        perror("Error creating consumer thread");
        exit(EXIT_FAILURE);
    }

    // Join threads
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores
    sem_destroy(&producer_sem);
    sem_destroy(&consumer_sem);

    // Destroy the circular queue
    destroy(queue);

    return 0;
}
