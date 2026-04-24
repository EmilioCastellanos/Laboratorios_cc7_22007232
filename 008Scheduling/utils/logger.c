#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "logger.h"

static pthread_mutex_t log_mutex;

void logger_init() {
    pthread_mutex_init(&log_mutex, NULL);
}

void log_event(Process *p, const char *msg) {
    pthread_mutex_lock(&log_mutex);
    
    time_t now = time(NULL);
    char *t_str = ctime(&now);
    t_str[strlen(t_str) - 1] = '\0';

    if (p != NULL) {
        printf("[%s] Process %d (Burst %.0f): %s\n", 
                t_str, p->id, p->burst_time, msg);
    } else {
        printf("[%s] %s\n", t_str, msg);
    }
    
    pthread_mutex_unlock(&log_mutex);
}

void log_header(const char *header) {
    pthread_mutex_lock(&log_mutex);
    printf("\n--- %s ---\n", header);
    pthread_mutex_unlock(&log_mutex);
}

void log_arrival_started(Process *p, double wait_time) {
    pthread_mutex_lock(&log_mutex);
    time_t now = time(NULL);
    char *t_str = ctime(&now);
    t_str[strlen(t_str) - 1] = '\0';

    printf("[%s] Process %d (Burst %.0f): Arrived at %.0f, Started (waited %.2f seconds)\n", 
            t_str, p->id, p->burst_time, p->arrival_time, wait_time);
    
    pthread_mutex_unlock(&log_mutex);
}

void log_fifo_start(Process *p, double wait_time) {
    pthread_mutex_lock(&log_mutex);
    time_t now = time(NULL);
    char *t_str = ctime(&now);
    t_str[strlen(t_str) - 1] = '\0';

    printf("[%s] Process %d (Burst %.0f): Arrived at %.0f, Started (waited %.2f seconds)\n", 
            t_str, p->id, p->burst_time, p->arrival_time, wait_time);
    
    pthread_mutex_unlock(&log_mutex);
}

void logger_destroy() {
    pthread_mutex_destroy(&log_mutex);
}