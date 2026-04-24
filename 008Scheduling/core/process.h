#ifndef PROCESS_H
#define PROCESS_H

#include <pthread.h>
#include <sys/time.h>

typedef struct {
    int id;
    double arrival_time;
    double burst_time;
    double remaining_time;
    
    // para cálculos
    double start_time;
    double completion_time;
    double waiting_time;
    double turnaround_time;

    pthread_t thread_id;
} Process;

#endif