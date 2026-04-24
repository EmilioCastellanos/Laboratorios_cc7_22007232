#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../core/process.h"
#include "../core/clock.h"
#include "../utils/logger.h"

int compare_arrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return (p1->arrival_time > p2->arrival_time) - (p1->arrival_time < p2->arrival_time);
}

void run_fifo(Process *processes, int n) {
    qsort(processes, n, sizeof(Process), compare_arrival);
    
    clock_reset();

    for (int i = 0; i < n; i++) {
        Process *p = &processes[i];

        // adelantar el reloj si no llega a la hora de arrive del proceso
        if (clock_get() < p->arrival_time) {
            double gap = p->arrival_time - clock_get();
            clock_tick(gap);
        }

        p->start_time = clock_get();
        double wait_time = p->start_time - p->arrival_time;

        log_fifo_start(p, wait_time);

        // cosillas para q no vaya volando verga
        usleep(p->burst_time * 100000); 
        
        // avanzar lo que el burst diga
        clock_tick(p->burst_time);
        
        p->completion_time = clock_get();
        log_event(p, "Completed");
    }
}