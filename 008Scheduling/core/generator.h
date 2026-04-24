#ifndef GENERATOR_H
#define GENERATOR_H

#include "process.h"

// genera un número aleatorio de procesos y les asigna valores random
int generate_workload(Process **processes);

// crea una copia de los threads para que cada algoritmo trabaje con los mismos
Process* clone_workload(Process *original, int num_processes);

#endif