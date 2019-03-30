#include <stdint.h>
#include <stdio.h>
#include "stm8s.h"
#include "delay.h"
#include "time.h"
#include "uart.h"
#include "cc1101.h"
#include "cdma_code.h"
#include "watchdog.h"
#include "awu.h"

/*
 * Redirect stdout to UART
 */
int putchar(int c) {
    uart_write(c);
    return 0;
}

/*
 * Redirect stdin to UART
 */
int getchar() {
    return uart_read();
}

static uint8_t awu_toggle;
/*
* Interrupt handler for auto-wakeup
*   We need to wake up at a higher frequency to enable both auto-wakeup
*   and watchdog together, since watchdog refresh time is limited.
*/
void awu_irq_handler() __interrupt(AWU_ISR) {
    // Read CSR to clear auto-wakeup flag
    uint8_t dummy;
    dummy = AWU_CSR;
    if (0 == awu_toggle) {
        iwdg_refresh();
        awu_toggle = 1;
        active_halt();
    }
    else {
        awu_toggle = 0;
    }
}

void main() {
    uint8_t counter = 0;

    // Enable interrupts
    enable_interrupts();

    // Enable watchdog
    iwdg_enable();

    // Enable auto wake up
    awu_toggle = 0;
    awu_enable();

    // Set stm8 internal clock divider to run with 16 MHz
    CLK_CKDIVR = 0;
    delay_ms(1);

    // Initialize UART for debugging
    uart_init();
    delay_ms(10);

    // Reset and Initialize CC1101
    printf("\nInitializing CC1101...\n");
    cc1101_init(CC1101_DATARATE_FAST);
    //cc1101_setSyncWord(syncWord[0],syncWord[1]);
    printf("CC1101 initialization complete.\n");
    // Print sum device information
    printf("CC1101_PARTNUM: ");
    printf("%d\n",cc1101_readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
    printf("CC1101_VERSION: ");
    printf("%d\n",cc1101_readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
    printf("CC1101_MARCSTATE: ");
    printf("%d\n",cc1101_readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);


    // Permanently toggle between sleep and burst tranmission until power source is dead
       printf("Beginning transmission.\n");
    while (1) {
        iwdg_refresh();
          cc1101_sendDataPollGdo0(cdma_code, CDMA_CODE_BYTES);
        printf("Transmitted code sequence %d time(s)\n", ++counter);
        // Refresh watchdog
        iwdg_refresh();
        // Go to sleep
        printf("Send MCU to active-halt.\n\n");
        active_halt();
    }
}
