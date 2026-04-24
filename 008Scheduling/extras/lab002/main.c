#include "stdio.h"

/* Prototipo de la función de limpieza */
void uart_flush(); 

void main() {
    int a, b;

    uart_flush(); /* Limpia el buffer antes de empezar */
    PRINT("Program: Calculator\n");

    while (1) {
        PRINT("Enter first number: ");
        READ("%d", &a);

        PRINT("Enter second number: ");
        READ("%d", &b);

        PRINT("Sum: %d\n", a + b);
    }
}