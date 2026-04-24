#include "clock.h"

static SystemClock sys_clock;

void clock_init() {
    sys_clock.current_time = 0;
    pthread_mutex_init(&sys_clock.mutex, NULL);
}

void clock_tick(double increment) {
    pthread_mutex_lock(&sys_clock.mutex);
    sys_clock.current_time += increment;
    pthread_mutex_unlock(&sys_clock.mutex);
}

double clock_get() {
    pthread_mutex_lock(&sys_clock.mutex);
    double t = sys_clock.current_time;
    pthread_mutex_unlock(&sys_clock.mutex);
    return t;
}

void clock_reset() {
    pthread_mutex_lock(&sys_clock.mutex);
    sys_clock.current_time = 0;
    pthread_mutex_unlock(&sys_clock.mutex);
}

void clock_destroy() {
    pthread_mutex_destroy(&sys_clock.mutex);
}