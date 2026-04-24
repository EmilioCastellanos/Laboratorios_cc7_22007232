#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../core/process.h"
#include "../core/clock.h"
#include "../utils/logger.h"

void run_srtf(Process *processes, int n) {
    clock_reset();
    int completed = 0;
    int current_idx = -1;
    
    int *started = calloc(n, sizeof(int));

    while (completed < n) {
        int shortest_idx = -1;
        double min_remaining = 999999;
        double current_time = clock_get();

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time < min_remaining) {
                    min_remaining = processes[i].remaining_time;
                    shortest_idx = i;
                }
            }
        }

        if (shortest_idx != -1) {
            if (current_idx != -1 && current_idx != shortest_idx && processes[current_idx].remaining_time > 0) {
                char msg[50];
                sprintf(msg, "Preempted (%.0f remaining)", processes[current_idx].remaining_time);
                log_event(&processes[current_idx], msg);
            }

            Process *p = &processes[shortest_idx];

            if (!started[shortest_idx]) {
                p->start_time = current_time;
                double wait_time = p->start_time - p->arrival_time;
                log_fifo_start(p, wait_time);
                started[shortest_idx] = 1;
            } else if (current_idx != shortest_idx) {
                log_event(p, "Resumed");
            }

            current_idx = shortest_idx;

            usleep(100000);
            clock_tick(1.0);
            p->remaining_time -= 1.0;

            if (p->remaining_time <= 0) {
                p->completion_time = clock_get();
                log_event(p, "Completed");
                completed++;
                current_idx = -1;
            }
        } else {
            clock_tick(1.0);
        }
    }
    free(started);
}