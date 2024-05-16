#include <stdint.h>
#include "kernel.c"

// Define constants for UART and GPIO register addresses
#define UART0_DR     (UART0_BASE + 0x00)
#define UART0_FR     (UART0_BASE + 0x18)

#define GPIO_BASE    0xFE200000
#define GPPUD        (GPIO_BASE + 0x94)
#define GPPUDCLK0    (GPIO_BASE + 0x98)

// Function to write a character to UART
void uart_putc(unsigned char c) {
    // Wait until UART FR register indicates UART is ready to transmit
    while (*(volatile uint32_t*)(UART0_FR) & (1 << 5)) { }
    // Write the character to UART data register
    *(volatile uint32_t*)(UART0_DR) = c;
}

// Function to read a character from UART
unsigned char uart_getc() {
    // Wait until UART FR register indicates UART has received data
    while (*(volatile uint32_t*)(UART0_FR) & (1 << 4)) { }
    // Read the character from UART data register
    return (unsigned char)(*(volatile uint32_t*)(UART0_DR));
}

// Function to initialize UART
void uart_init() {
    // Turn off UART while we configure it
    mmio_write(UART0_CR, 0x00000000);

    // Configure GPIO for UART
    mmio_write(GPPUD, 0x00000000);
    delay(150);
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    mmio_write(GPPUDCLK0, 0x00000000);

    // Clear pending interrupts
    mmio_write(UART0_ICR, 0x7FF);

    // Configure baud rate (115200 bps)
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    // Configure line control
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // Enable necessary interrupts
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    // Enable UART
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
