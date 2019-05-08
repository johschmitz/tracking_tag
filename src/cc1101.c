#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm8s.h"
#include "delay.h"
#include "gpio.h"
#include "cc1101.h"
#include "spi.h"


void cc1101_init(enum DATARATE datarate, enum MODULATION modulation) {
    // Set CSN_PIN as output pin
    PinOutput(CSN_PORT,CSN_PIN);
    PinOutputPullUp(CSN_PORT,CSN_PIN);
    PinOutputSpeed2MHZ(CSN_PORT,CSN_PIN);

    PinInput(GD0_PORT,GD0_PIN);
    PinInputFloat(GD0_PORT,GD0_PIN);

    // GD1
    PinInput(SO_PORT,SO_PIN);
    PinInputFloat(SO_PORT,SO_PIN);

    PinInput(GD2_PORT,GD2_PIN);
    PinInputFloat(GD2_PORT,GD2_PIN);

    PinOutput(SI_PORT,SI_PIN);
    PinOutputPullUp(SI_PORT,SI_PIN);
    PinOutputSpeed2MHZ(SI_PORT,SI_PIN);

    SPI_init();
    cc1101_reset();
    cc1101_setCCregs(datarate, modulation);
}

void cc1101_reset() {
    PinLow(CSN_PORT,CSN_PIN);
    _delay_us(5);
    PinHigh(CSN_PORT,CSN_PIN);
    _delay_us(30);
    PinLow(CSN_PORT,CSN_PIN);
    _delay_us(50);
    cc1101_select();
    cc1101_writeCmdStrobe(CC1101_SRES);
    cc1101_unSelect();
    printf("CC1101 reset complete.\n");
}

void cc1101_setCCregs(enum DATARATE datarate, enum MODULATION modulation) {
    cc1101_writeSingle(CC1101_IOCFG2,   CC1101_DEFVAL_IOCFG2);
    cc1101_writeSingle(CC1101_IOCFG1,   CC1101_DEFVAL_IOCFG1);
    cc1101_writeSingle(CC1101_IOCFG0,   CC1101_DEFVAL_IOCFG0);
    cc1101_writeSingle(CC1101_FIFOTHR,  CC1101_DEFVAL_FIFOTHR);
    cc1101_writeSingle(CC1101_PKTLEN,   CC1101_DEFVAL_PKTLEN);
    cc1101_writeSingle(CC1101_PKTCTRL1, CC1101_DEFVAL_PKTCTRL1);
    cc1101_writeSingle(CC1101_PKTCTRL0, CC1101_DEFVAL_PKTCTRL0);
    cc1101_writeSingle(CC1101_SYNC1, CC1101_DEFVAL_SYNC1);
    cc1101_writeSingle(CC1101_SYNC0, CC1101_DEFVAL_SYNC0);
    cc1101_writeSingle(CC1101_ADDR, CC1101_DEFVAL_ADDR);
    cc1101_writeSingle(CC1101_CHANNR,  CC1101_DEFVAL_CHANNR);
    cc1101_writeSingle(CC1101_FSCTRL1,  CC1101_DEFVAL_FSCTRL1);
    cc1101_writeSingle(CC1101_FSCTRL0,  CC1101_DEFVAL_FSCTRL0);
    cc1101_writeSingle(CC1101_FREQ2,  CC1101_DEFVAL_FREQ2_433);
    cc1101_writeSingle(CC1101_FREQ1,  CC1101_DEFVAL_FREQ1_433);
    cc1101_writeSingle(CC1101_FREQ0,  CC1101_DEFVAL_FREQ0_433);
    if (CC1101_DATARATE_SLOW == datarate) {
        printf("CC1101 setting data rate to slow.\n");
        cc1101_writeSingle(CC1101_MDMCFG4,  CC1101_DEFVAL_MDMCFG4_SLOW);
        cc1101_writeSingle(CC1101_MDMCFG3,  CC1101_DEFVAL_MDMCFG3_SLOW);
    }
    else {
        printf("CC1101 setting data rate to fast.\n");
        cc1101_writeSingle(CC1101_MDMCFG4,  CC1101_DEFVAL_MDMCFG4_FAST);
        cc1101_writeSingle(CC1101_MDMCFG3,  CC1101_DEFVAL_MDMCFG3_FAST);
    }
    if (CC1101_MODULATION_FSK == modulation) {
        cc1101_writeSingle(CC1101_MDMCFG2,  CC1101_DEFVAL_MDMCFG2_FSK);
    }
    else {
        cc1101_writeSingle(CC1101_MDMCFG2,  CC1101_DEFVAL_MDMCFG2_OOK);
    }
    cc1101_writeSingle(CC1101_MDMCFG1,  CC1101_DEFVAL_MDMCFG1);
    cc1101_writeSingle(CC1101_MDMCFG0,  CC1101_DEFVAL_MDMCFG0);
    cc1101_writeSingle(CC1101_DEVIATN,  CC1101_DEFVAL_DEVIATN);
    cc1101_writeSingle(CC1101_MCSM2,  CC1101_DEFVAL_MCSM2);
    cc1101_writeSingle(CC1101_MCSM1,  CC1101_DEFVAL_MCSM1);
    cc1101_writeSingle(CC1101_MCSM0,  CC1101_DEFVAL_MCSM0);
    cc1101_writeSingle(CC1101_FOCCFG,  CC1101_DEFVAL_FOCCFG);
    cc1101_writeSingle(CC1101_BSCFG,  CC1101_DEFVAL_BSCFG);
    cc1101_writeSingle(CC1101_AGCCTRL2,  CC1101_DEFVAL_AGCCTRL2);
    cc1101_writeSingle(CC1101_AGCCTRL1,  CC1101_DEFVAL_AGCCTRL1);
    cc1101_writeSingle(CC1101_AGCCTRL0,  CC1101_DEFVAL_AGCCTRL0);
    cc1101_writeSingle(CC1101_WOREVT1,  CC1101_DEFVAL_WOREVT1);
    cc1101_writeSingle(CC1101_WOREVT0,  CC1101_DEFVAL_WOREVT0);
    cc1101_writeSingle(CC1101_WORCTRL,  CC1101_DEFVAL_WORCTRL);
    cc1101_writeSingle(CC1101_FREND1,  CC1101_DEFVAL_FREND1);
    if (CC1101_MODULATION_FSK == modulation) {    
        cc1101_writeSingle(CC1101_FREND0,  CC1101_DEFVAL_FREND0_FSK);
    }
    else {
        cc1101_writeSingle(CC1101_FREND0,  CC1101_DEFVAL_FREND0_OOK);
    }
    if (CC1101_DATARATE_SLOW == datarate) {
        cc1101_writeSingle(CC1101_FSCAL3,  CC1101_DEFVAL_FSCAL3_SLOW);
    }
    else {
        cc1101_writeSingle(CC1101_FSCAL3,  CC1101_DEFVAL_FSCAL3_FAST);    
    }
    cc1101_writeSingle(CC1101_FSCAL2,  CC1101_DEFVAL_FSCAL2);
    cc1101_writeSingle(CC1101_FSCAL1,  CC1101_DEFVAL_FSCAL1);
    cc1101_writeSingle(CC1101_FSCAL0,  CC1101_DEFVAL_FSCAL0);
    cc1101_writeSingle(CC1101_RCCTRL1,  CC1101_DEFVAL_RCCTRL1);
    cc1101_writeSingle(CC1101_RCCTRL0,  CC1101_DEFVAL_RCCTRL0);
    cc1101_writeSingle(CC1101_FSTEST,  CC1101_DEFVAL_FSTEST);
    cc1101_writeSingle(CC1101_PTEST,  CC1101_DEFVAL_PTEST);
    cc1101_writeSingle(CC1101_AGCTEST,  CC1101_DEFVAL_AGCTEST);
    cc1101_writeSingle(CC1101_TEST2,  CC1101_DEFVAL_TEST2);
    cc1101_writeSingle(CC1101_TEST1,  CC1101_DEFVAL_TEST1);
    cc1101_writeSingle(CC1101_TEST0,  CC1101_DEFVAL_TEST0);
    if (CC1101_MODULATION_FSK == modulation) {
        cc1101_writeSingle(CC1101_PATABLE, CC1101_PA_10_DBM);
    }
    else {
        cc1101_writePaTableOok(CC1101_PA_10_DBM);
    }
    
    printf("CC1101 configuration complete.\n");
}

void cc1101_waitReady() {
    // Wait for CC1101 to become ready
    while(PinRead(SO_PORT,SO_PIN));
}

void cc1101_select() {
    PinLow(CSN_PORT,CSN_PIN);
    cc1101_waitReady();
}

void cc1101_unSelect() {
    PinHigh(CSN_PORT,CSN_PIN);
}

void wait_GDO0_high() {
    while (!PinRead(GD0_PORT,GD0_PIN));
}

void wait_GDO0_low() {
    while(PinRead(GD0_PORT,GD0_PIN));
}

uint8_t cc1101_readReg(uint8_t address, uint8_t regType) {
    uint8_t rv;
    cc1101_select();
    SPI_write(address | regType);
    rv = SPI_write(0x00); // read actual value;
    cc1101_unSelect();
    return  rv;
}

uint8_t cc1101_readStatus(uint8_t address){
    uint8_t rv;
    cc1101_select();
    SPI_write(address | READ_BURST);
    // read actual value
    rv = SPI_write(0x00);
    cc1101_unSelect();
    return  rv;
}

void cc1101_writeSingle(uint8_t address, uint8_t value) {
    cc1101_select();
    SPI_write(address);
    SPI_write(value);
    cc1101_unSelect();
}

void cc1101_writeBurstCDMA(uint8_t address, uint8_t *chips, uint8_t databitPacked, uint8_t len) {
    int i;
    cc1101_select();
    SPI_write(address | WRITE_BURST);
    for(i=0;i<len;i++) {
        SPI_write(chips[i]^databitPacked);
    }
    cc1101_unSelect();
}

void cc1101_writeCmdStrobe(uint8_t command ){
    cc1101_select();
    SPI_write(command);
    cc1101_unSelect();
}

void cc1101_writePaTableOok(uint8_t paValue)
{
    uint8_t pa_values[CC1101_PATABLE_LEN] = {0x00,paValue,0x00,0x00,0x00,0x00,0x00,0x00};
    int i;
    cc1101_select();
    for(i=0;i<CC1101_PATABLE_LEN;i++) {
        SPI_write(CC1101_PATABLE);
        SPI_write(pa_values[i]);
    }
    cc1101_unSelect();
}

void cc1101_synthStartupState()
{
    cc1101_writeCmdStrobe(CC1101_SFSTXON);
}

void cc1101_setTxState()
{
    cc1101_writeCmdStrobe(CC1101_STX);
}

void cc1101_flushTxFifo() {
    cc1101_writeCmdStrobe(CC1101_SFTX);
}

void cc1101_setIdleState()
{
    cc1101_writeCmdStrobe(CC1101_SIDLE);
}

void cc1101_setSleepState()
{
    cc1101_writeCmdStrobe(CC1101_SPWD);
}

void cc1101_sendDataPollGdo0(uint8_t *chips, uint16_t numCDMABytes,
                             uint8_t *data, uint8_t databitCounter,
                             enum MODULATION modulation)
{
    uint16_t byteCounter = 0;
    uint8_t databitUnpacked;
    uint8_t databitPacked;
    enum MARCSTATE marcState;

    if (CC1101_MODULATION_OOK == modulation) {
        // For OOK PATABLE needs to be set after each SLEEP
        cc1101_writePaTableOok(CC1101_PA_10_DBM);
    }

    printf("Startup frequency synthesizer.\n");
    cc1101_synthStartupState();
    delay_ms(1);

    // Read databit and pack for XOR operation with CDMA chips
    databitUnpacked = data[(int)(databitCounter/8)] >> (7 - databitCounter % 8) & 0x01 ;
    printf("Databit: %u\n",databitUnpacked);
    if (1 == databitUnpacked) {
        databitPacked = 0x00;
    }
    else {
        databitPacked = 0xFF;
    }

    printf("Length of CDMA sequence: %u\n", numCDMABytes*8);
    if (numCDMABytes > 0)
    {
        printf("Beginning TX burst...\n");
        // Set infinite package length mode
        cc1101_writeSingle(CC1101_PKTCTRL0,CC1101_PKT_INF_LEN);
        cc1101_writeSingle(CC1101_PKTLEN,0);
        cc1101_writeBurstCDMA(CC1101_TXFIFO, chips, databitPacked, CC1101_TXFIFO_LEN);
        byteCounter += CC1101_TXFIFO_LEN;
        cc1101_setTxState();
        if (byteCounter < numCDMABytes) {
            // Refill TX FIFO periodically until sequence is complete
            do {
                wait_GDO0_low();
                cc1101_writeBurstCDMA(CC1101_TXFIFO, &chips[byteCounter], databitPacked, CC1101_TXFIFO_REFILL);
                byteCounter += CC1101_TXFIFO_REFILL;
                // Check for TX FIFO underflow
                if (TXFIFO_UNDERFLOW == cc1101_readStatus(CC1101_MARCSTATE) & 0x1F) {
                    printf("TXFIFO underflow. Go back to IDLE state.\n");
                    cc1101_flushTxFifo();
                    cc1101_setIdleState();
                    return;
                }
            } while(byteCounter < numCDMABytes);
        }
        cc1101_writeSingle(CC1101_PKTCTRL0,CC1101_PKT_FIXED_LEN);
    }
    do {
        // Wait for IDLE state
        marcState = cc1101_readStatus(CC1101_MARCSTATE) & 0x1F;
        printf("MARCSTATE after TX FIFO write loop: %u\n", marcState);
    } while (IDLE != marcState);
    printf("TX burst complete. Send CC1101 to SLEEP.\n");
    cc1101_setSleepState();
}