#ifndef CLOCK_H
#define CLOCK_H

#include <pthread.h>

typedef struct {
    double current_time;
    pthread_mutex_t mutex;
} SystemClock;


void clock_init();
void clock_tick(double increment);
double clock_get();
void clock_reset();
void clock_destroy();

#endif