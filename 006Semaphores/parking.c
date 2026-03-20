#include "parking.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

// Semaphores
sem_t parking_sem;
sem_t log_mutex;
sem_t stats_mutex;

// Stats
int total_cars_parked = 0;
double total_wait_time = 0.0;

// Timestamp helper
void print_timestamp() {
    time_t now = time(NULL);
    char* t = ctime(&now);
    t[strlen(t) - 1] = '\0'; // quitar \n
    printf("[%s] ", t);
}

// Init
void init_parking() {
    sem_init(&parking_sem, 0, NUM_SPOTS);
    sem_init(&log_mutex, 0, 1);
    sem_init(&stats_mutex, 0, 1);
}

// Destroy
void destroy_parking() {
    sem_destroy(&parking_sem);
    sem_destroy(&log_mutex);
    sem_destroy(&stats_mutex);
}

// Arrival
void car_arrives(int car_id) {
    sem_wait(&log_mutex);
    print_timestamp();
    printf("Car %d: Arrived at parking lot\n", car_id);
    sem_post(&log_mutex);
}

// Parking
void car_parks(int car_id, double wait_time) {
    sem_wait(&log_mutex);
    print_timestamp();
    printf("Car %d: Parked successfully (waited %.2f seconds)\n", car_id, wait_time);
    sem_post(&log_mutex);

    sem_wait(&stats_mutex);
    total_cars_parked++;
    total_wait_time += wait_time;
    sem_post(&stats_mutex);
}

// Leaving
void car_leaves(int car_id) {
    sem_wait(&log_mutex);
    print_timestamp();
    printf("Car %d: Leaving parking lot\n", car_id);
    sem_post(&log_mutex);
}

// Stats print
void print_stats() {
    printf("\nTotal cars parked: %d\n", total_cars_parked);
    printf("Average wait time: %.2f seconds\n",
           total_wait_time / total_cars_parked);
}