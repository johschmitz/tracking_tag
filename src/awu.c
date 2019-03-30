#include "awu.h"
#include <stm8s.h>

/*
* Enable auto wake up
*/

void awu_enable() {
    // Timebase 0b1011 (512 ms - 1.024 s)
    AWU_TBR = 0x0B;
    // Prescaler 62 (with 1011 timebase -> 0,496 s)
    AWU_APR = 0x3E;
    // Enable the auto wake up feature
    AWU_CSR = AWU_CSR | (1 << AWU_CSR_AWUEN);
}    

void awu_disable() {
    // Disable the auto wake up feature
    AWU_CSR = AWU_CSR | (0 << AWU_CSR_AWUEN);
    AWU_TBR = 0;
}

void active_halt() {
    // Switch off main regulator during halt mode
    CLK_ICKR = CLK_ICKR | (1 << CLK_ICKR_REGAH);
    // Power down flash during halt mode
    FLASH_CR1 = FLASH_CR1 | (1 << FLASH_CR1_AHALT);
    // Halt instruction
    halt();
}