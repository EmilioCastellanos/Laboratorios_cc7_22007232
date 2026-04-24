#define UART0_BASE 0x44E09000

#define UART_THR      0x00
#define UART_LSR      0x14
#define UART_LSR_THRE 0x20

volatile unsigned int * const UART0 = (unsigned int *)UART0_BASE;

void uart_putc(char c) {
    while (!(UART0[UART_LSR / 4] & UART_LSR_THRE));
    UART0[UART_THR / 4] = c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void main() {
    uart_puts("Hello World\r\n");
    while (1);
}