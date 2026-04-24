#include "os.h"

#define UART0_BASE 0x44E09000
#define UART_RHR      0x00
#define UART_THR      0x00
#define UART_LSR      0x14
#define UART_LSR_THRE 0x20
#define UART_LSR_DR   0x01

volatile unsigned int * const UART0 = (unsigned int *)UART0_BASE;

void uart_putc(char c) {
    while (!(UART0[UART_LSR / 4] & UART_LSR_THRE));
    UART0[UART_THR / 4] = c;
}

char uart_getc() {
    while (!(UART0[UART_LSR / 4] & UART_LSR_DR));
    return (char)(UART0[UART_RHR / 4] & 0xFF);
}

/* Nueva función para limpiar basura del buffer */
void uart_flush() {
    while (UART0[UART_LSR / 4] & UART_LSR_DR) {
        volatile char dummy = (char)UART0[UART_RHR / 4];
    }
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void uart_gets_input(char *buffer, int max_length) {
    int i = 0;
    char c;
    
    while (i < max_length - 1) {
        c = uart_getc();
        
        /* Ignorar caracteres de control iniciales como \n o \r residuales */
        if (i == 0 && (c == '\n' || c == '\r')) continue;

        if (c == '\r' || c == '\n') {
            uart_putc('\r');
            uart_putc('\n');
            break;
        }

        uart_putc(c);
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

int uart_atoi(const char *s) {
    int num = 0;
    int sign = 1;
    int i = 0;
    while (s[i] == ' ') i++;
    if (s[i] == '-') {
        sign = -1;
        i++;
    }
    for (; s[i] >= '0' && s[i] <= '9'; i++) {
        num = num * 10 + (s[i] - '0');
    }
    return sign * num;
}

void uart_itoa(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;
    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    while (num > 0 && i < 14) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    if (is_negative) buffer[i++] = '-';
    buffer[i] = '\0';
    int start = 0, end = i - 1;
    char temp;
    while (start < end) {
        temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void OS_WRITE(const char *s) {
    uart_puts(s);
}

void OS_READ(char *buffer, int max_length) {
    uart_gets_input(buffer, max_length);
}

void OS_ITOA(int num, char *buffer) {
    uart_itoa(num, buffer);
}

int OS_ATOI(const char *s) {
    return uart_atoi(s);
}