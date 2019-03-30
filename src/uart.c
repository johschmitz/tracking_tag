#include "uart.h"
#include "stm8s.h"
#include "delay.h"

void uart_init() {
    // Round to nearest integer
    uint16_t div = (F_CPU + BAUDRATE / 2) / BAUDRATE;
    // Madness..
    UART1_BRR2 = ((div >> 8) & 0xF0) + (div & 0x0F);
    UART1_BRR1 = div >> 4;
    // Enable transmitter and receiver
    UART1_CR2 = (1 << UART1_CR2_TEN) | (1 << UART1_CR2_REN);
    // Wait a bit for settings to become active
    delay_ms(1);    
}

void uart_write(uint8_t data) {
    UART1_DR = data;
    while (!(UART1_SR & (1 << UART1_SR_TC)));
}

uint8_t uart_read() {
    while (!(UART1_SR & (1 << UART1_SR_RXNE)));
    return UART1_DR;
}
