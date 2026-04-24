#include "process.h"
#include <stdio.h>

void calculate_metrics(Process* processes, int num_threads) {
    double total_wt = 0;
    double total_tat = 0;

    printf("\n--- Estadísticas Finales ---\n");
    for (int i = 0; i < num_threads; i++) {

        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;

        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        total_wt += processes[i].waiting_time;
        total_tat += processes[i].turnaround_time;

        printf("Proceso %d | WT: %.2f s | TAT: %.2f s\n", 
                processes[i].id, processes[i].waiting_time, processes[i].turnaround_time);
    }

    printf("\nAvg Waiting Time: %.2f seconds\n", total_wt / num_threads);
    printf("Avg Turnaround Time: %.2f seconds\n", total_tat / num_threads);
}