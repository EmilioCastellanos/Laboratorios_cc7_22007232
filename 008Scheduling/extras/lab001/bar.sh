#!/bin/bash

set -e

echo "Limpiando archivos antiguos..."
rm -f *.o *.elf *.bin

echo "Ensamblando startup.s..."
arm-none-eabi-as -o startup.o startup.s

echo "Compilando hello.c..."
arm-none-eabi-gcc -c -o hello.o hello.c

echo "Enlazando archivos de objeto..."

arm-none-eabi-ld -T memmap.ld -o hello.elf startup.o hello.o

echo "Generando archivo binario..."
arm-none-eabi-objcopy -O binary hello.elf hello.bin

echo "Listo. Carga 'hello.bin' en la BeagleBone Black."