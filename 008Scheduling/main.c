#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "core/process.h"
#include "core/generator.h"
#include "utils/logger.h"

void run_fifo(Process *processes, int n);
void run_rr(Process *processes, int n, int quantum);
void run_sjf(Process *processes, int n);
void run_srtf(Process *processes, int n);

extern void calculate_metrics(Process* processes, int num_threads);

int main() {
    srand(time(NULL));
    logger_init();

    Process *master_data = NULL;
    int n = generate_workload(&master_data);

    printf("Dataset generado: %d procesos listos para la simulación.\n", n);

    // --- SIMULACIÓN FIFO ---
    log_header("First-In-First-Out (FIFO) Scheduling");
    Process *fifo_copy = clone_workload(master_data, n);
    run_fifo(fifo_copy, n);
    calculate_metrics(fifo_copy, n);
    free(fifo_copy);

   
    int Q = 5;
    printf("\n--- Round Robin Scheduling (Quantum %d) ---\n", Q);
    Process *rr_copy = clone_workload(master_data, n);
    run_rr(rr_copy, n, Q);
    calculate_metrics(rr_copy, n);
    free(rr_copy);

    // --- SIMULACIÓN SJF ---
    log_header("Shortest Job First (SJF) Scheduling");
    Process *sjf_copy = clone_workload(master_data, n);
    run_sjf(sjf_copy, n);
    calculate_metrics(sjf_copy, n);
    free(sjf_copy);

    // --- SIMULACIÓN SRTF ---
    log_header("Shortest Remaining Time First (SRTF) Scheduling");
    Process *srtf_copy = clone_workload(master_data, n);
    run_srtf(srtf_copy, n);
    calculate_metrics(srtf_copy, n);
    free(srtf_copy);

    // 3. Limpieza final
    free(master_data);
    logger_destroy();

    printf("\nSimulaciones completadas exitosamente.\n");
    return 0;
}