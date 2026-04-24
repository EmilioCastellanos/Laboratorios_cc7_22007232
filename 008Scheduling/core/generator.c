#include "generator.h"
#include <stdlib.h>
#include <time.h>

int generate_workload(Process **processes) {
    // generar entre 5 y 15 hilos
    int num_processes = (rand() % 11) + 5; 
    
    *processes = malloc(sizeof(Process) * num_processes);

    for (int i = 0; i < num_processes; i++) {
        (*processes)[i].id = i;
        // burst aleatorio: 1-10 segundos
        (*processes)[i].burst_time = (rand() % 10) + 1; 
        (*processes)[i].remaining_time = (*processes)[i].burst_time;
        // tiempo de llegada: 0-100 segundos
        (*processes)[i].arrival_time = rand() % 101; 
        
        // inicializar métricas
        (*processes)[i].start_time = -1; 
        (*processes)[i].completion_time = 0;
    }
    
    return num_processes;
}

Process* clone_workload(Process *original, int num_processes) {
    Process *copy = malloc(sizeof(Process) * num_processes);
    for (int i = 0; i < num_processes; i++) {
        copy[i] = original[i];
    }
    return copy;
}