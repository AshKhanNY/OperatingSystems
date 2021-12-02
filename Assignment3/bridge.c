/* ----- ASSIGNMENT 3 (BRIDGE) -----
Implement a solution using Pthreads that synchronizes 
the threads access to the output screen and prevents 
deadlock. In particular, represent northbound and 
southbound farmers as separate threads (use several 
threads representing the northbound and 
southbound farmers). 

>>> HOW TO COMPILE AND RUN <<<

gcc -pthread -o bridge bridge.c
./bridge

*/ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define THREADS 4

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct alternator {
    int count;
    char* bridge;
};

void *runner(void *alt);

int main() {
    // 4 Threads set up for each side (North and South)
    pthread_t north[THREADS], south[THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Initialize mutex lock
    pthread_mutex_init(&lock, NULL);

    // Alternator keeps track of which bridge to cross and
    // whether to go north bound or south
    struct alternator alt[2]; 
    alt[0].count = 0;
    alt[0].bridge = "North Tunbridge";
    alt[1].count = 0;
    alt[1].bridge = "South Tunbridge";

    // Create bridges for each thread
    for(int i = 0; i < THREADS;i++) {
        pthread_create(&north[i], &attr, runner, (void *)&alt[0]);
        pthread_create(&south[i], &attr, runner, (void *)&alt[1]);
    }

    // Wait for all threads to finish up
    for (int i = 0; i < THREADS;i++) {
        pthread_join(north[i], NULL);
        pthread_join(south[i], NULL);
    }

    // Free lock, since it's no longer needed
    pthread_mutex_destroy(&lock); 
}

void *runner(void *alt) {
    // Locks thread (commenting this line will allow you to run
    // this program without a lock)
    //pthread_mutex_lock(&lock);

    struct alternator *args = alt;
    // Keeping track of which bridge you're on
    args->count++;
    printf("%s #%d farmer can cross the bridge\n", args->bridge, args->count);

    // Random time, allows you to have varying seconds from 0-3
    // while travelling
    srand(time(NULL));
    printf("%s #%d is traveling on the bridge...\n", args->bridge, args->count);
    sleep(rand() % 3);
    printf("%s #%d farmer has left the bridge\n\n", args->bridge, args->count);

    // Unlocks thread (commenting this line will allow you to run
    // this program without a lock)
    //pthread_mutex_unlock(&lock);
    pthread_exit(0);
}