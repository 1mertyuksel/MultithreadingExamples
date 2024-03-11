#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE_ARRAY  150

void display_array(const int *pa, size_t size);
void bubble_sort(int *pa, size_t size);
void random_array(int *pa, size_t size);
void randoms();

pthread_mutex_t sort_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sort_cond = PTHREAD_COND_INITIALIZER;
int sort_flag;

int a[SIZE_ARRAY];

void *workerThread1(void *x) {
    printf("Thread1 is running..\n");
    pthread_mutex_lock(&sort_mutex);

    while (sort_flag == 0) {
        printf("Thread1 is waiting..\n");
        pthread_cond_wait(&sort_cond, &sort_mutex);
    }
    display_array(a, SIZE_ARRAY);

    pthread_mutex_unlock(&sort_mutex);

    return NULL;
}

void *workerThread2(void *x) {
    printf("Sorting Thread is running..\n");

    sleep(2);

    randoms();
    random_array(a, SIZE_ARRAY);
    bubble_sort(a, SIZE_ARRAY);

    pthread_mutex_lock(&sort_mutex);
    sort_flag = 1;
    pthread_mutex_unlock(&sort_mutex);
    
    pthread_cond_signal(&sort_cond);

    return NULL;
}

int main() {
    pthread_t threads[2];

    for (int i = 0; i < 2; ++i) {
        if (i == 0) {
            if (pthread_create(&threads[i], NULL, workerThread1, NULL) != 0) {
                perror("pthread_create error");
                return 1;
            }
        } else {
            if (pthread_create(&threads[i], NULL, workerThread2, NULL) != 0) {
                perror("pthread_create error");
                return 1;
            }
        }
    }

    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void bubble_sort(int *pa, size_t size) {
    for (int i = 0; i < size - 1; ++i)
        for (int j = 0; j < size - 1 - i; ++j) {
            if (pa[j] > pa[j + 1]) {
                int temp = pa[j];
                pa[j] = pa[j + 1];
                pa[j + 1] = temp;
            }
        }
}

void random_array(int *pa, size_t size) {
    for (int i = 0; i < size; ++i)
        pa[i] = rand() % 100;
}

void randoms() {
    time_t tmr;
    srand((unsigned int)(time(&tmr)));
}

void display_array(const int *pa, size_t size) {
    for (int i = 0; i < size; ++i)
        printf("%d ", pa[i]);

    printf("\n");
}
