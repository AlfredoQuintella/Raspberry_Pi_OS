#ifndef UART_H
#define UART_H

// Prototypes for UART functions
void uart_init();
void uart_putc(unsigned char c);
unsigned char uart_getc();

#endif /* UART_H */
