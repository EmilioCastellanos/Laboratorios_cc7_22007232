#!/bin/bash

BINARY="scheduler_sim"
INCLUDES="-I./core -I./utils -I./schedulers"
FLAGS="-Wall -Wextra -pthread"

# --- Limpieza ---
echo "--- Limpiando binarios anteriores ---"
rm -f $BINARY

# --- Compilación ---
echo "--- Compilando el proyecto ---"
gcc $FLAGS $INCLUDES \
    main.c \
    core/generator.c \
    core/metrics.c \
    core/clock.c \
    utils/logger.c \
    schedulers/fifo.c \
    schedulers/rr.c \
    schedulers/sjf.c \
    schedulers/srtf.c \
    -o $BINARY

# --- Ejecución ---
if [ $? -eq 0 ]; then
    echo -e "--- Ejecución Iniciada ---\n"
    ./$BINARY
else
    echo -e "\n[ERROR] La compilación falló. Revisa los mensajes de arriba."
    exit 1
fi