#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../core/process.h"
#include "../core/clock.h"
#include "../utils/logger.h"

void run_sjf(Process *processes, int n) {
    clock_reset();
    int completed = 0;
    
    int *is_done = calloc(n, sizeof(int));

    while (completed < n) {
        int shortest_idx = -1;
        double min_burst = 999999;
        double current_time = clock_get();

        for (int i = 0; i < n; i++) {
            if (!is_done[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    shortest_idx = i;
                }
            }
        }

        if (shortest_idx != -1) {
            Process *p = &processes[shortest_idx];
            
            p->start_time = current_time;
            double wait_time = p->start_time - p->arrival_time;
            
            log_fifo_start(p, wait_time);

            usleep(p->burst_time * 100000);
            clock_tick(p->burst_time);
            
            p->completion_time = clock_get();
            log_event(p, "Completed");
            
            is_done[shortest_idx] = 1;
            completed++;
        } else {
            clock_tick(1.0);
        }
    }
    free(is_done);
}