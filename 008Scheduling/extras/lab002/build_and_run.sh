#!/bin/bash

set -e

echo "Cleaning up..."
rm -f *.o *.elf *.bin

echo "Assembling..."
arm-none-eabi-as -o root.o root.s

echo "Compiling..."
arm-none-eabi-gcc -c -o os.o os.c
arm-none-eabi-gcc -c -o stdio.o stdio.c
arm-none-eabi-gcc -c -o main.o main.c

echo "Linking..."
arm-none-eabi-ld -T linker.ld -o calculadora.elf root.o os.o stdio.o main.o

echo "Generating Binary..."
arm-none-eabi-objcopy -O binary calculadora.elf calculadora.bin

echo "Done. Transfer calculadora.bin to your BeagleBone Black."