#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../core/process.h"
#include "../core/clock.h"
#include "../utils/logger.h"

void run_rr(Process *processes, int n, int quantum) {
    clock_reset();
    int completed = 0;
    double current_time;

    int *started = calloc(n, sizeof(int));

    while (completed < n) {
        int made_progress = 0;

        for (int i = 0; i < n; i++) {
            Process *p = &processes[i];
            current_time = clock_get();

            if (p->arrival_time <= current_time && p->remaining_time > 0) {
                made_progress = 1;

                if (!started[i]) {
                    p->start_time = current_time;
                    double wait_time = p->start_time - p->arrival_time;
                    log_fifo_start(p, wait_time);
                    started[i] = 1;
                } else {
                    log_event(p, "Resumed");
                }

                double run_time = (p->remaining_time > quantum) ? quantum : p->remaining_time;

                usleep(run_time * 100000); 
                clock_tick(run_time);
                p->remaining_time -= run_time;

                if (p->remaining_time > 0) {
                    char msg[50];
                    sprintf(msg, "Preempted (%.0f remaining)", p->remaining_time);
                    log_event(p, msg);
                } else {
                    p->completion_time = clock_get();
                    log_event(p, "Completed");
                    completed++;
                }
            }
        }

        if (!made_progress) {
            clock_tick(1.0); 
        }
    }
    free(started);
}