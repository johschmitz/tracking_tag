#include <stdint.h>
#include "stm8s.h"
#include "gpio.h"
#include "spi.h"
#include "stm8s.h"
#include "delay.h"

void SPI_init() {
    SPI_CR1 = 0b01000100;
    SPI_CR2 = 0b00000000;
}

uint8_t SPI_write(uint8_t data) {
    SPI_DR = data;
    while (!(SPI_SR & (1 << SPI_SR_TXE)));
    _delay_us(1);
    while (!(SPI_SR & (1 << SPI_SR_RXNE)));
    return SPI_DR;
}
