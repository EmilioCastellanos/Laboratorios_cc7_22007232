#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "parking.h"

extern sem_t parking_sem;

double get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

void* car_thread(void* arg) {
    int car_id = *(int*)arg;
    free(arg);

    sleep(rand() % 3 + 1);

    car_arrives(car_id);

    double start_wait = get_time();

    sem_wait(&parking_sem);

    double end_wait = get_time();
    double wait_time = end_wait - start_wait;

    car_parks(car_id, wait_time);

    sleep(rand() % MAX_WAIT_TIME + 1);

    car_leaves(car_id);

    sem_post(&parking_sem);

    return NULL;
}

int main() {
    srand(time(NULL));

    init_parking();

    pthread_t cars[NUM_CARS];

    for (int i = 0; i < NUM_CARS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;

        pthread_create(&cars[i], NULL, car_thread, id);
    }

    for (int i = 0; i < NUM_CARS; i++) {
        pthread_join(cars[i], NULL);
    }

    print_stats();
    destroy_parking();

    return 0;
}