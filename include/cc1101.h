
/**
 * Statemachine states
 */
enum MARCSTATE
{
  SLEEP = 0, IDLE, XOFF, VCOON_MC,
  REGON_MC, MANCAL, VCOON, REGON,
  STARTCAL, BWBOOST, FS_LOCK, IFADCON,
  ENDCAL, RX, RX_END, RX_RST,
  TXRX_SWITCH, RXFIFO_OVERFLOW, FSTXON, TX,
  TX_END, RXTX_SWITCH, TXFIFO_UNDERFLOW
};

/**
 * Data rate mode
 */
// SLOW -> 1.00112 kbps, FAST 1 Mbps
enum DATARATE
{
  CC1101_DATARATE_SLOW,
  CC1101_DATARATE_FAST
};

/**
 * Type of transfers
 */
#define WRITE_SINGLE             0x00
#define WRITE_BURST              0x40
#define READ_SINGLE              0x80
#define READ_BURST               0xC0

/**
 * Type of register
 */
#define CC1101_CONFIG_REGISTER   READ_SINGLE
#define CC1101_STATUS_REGISTER   READ_BURST

/**
 * PATABLE & FIFO's
 */
#define CC1101_PATABLE0          0x3E       // PATABLE address
#define CC1101_TXFIFO            0x3F       // TX FIFO address
#define CC1101_RXFIFO            0x3F       // RX FIFO address

/**
 * PATABLE values
 */
#define CC1101_PA_0_DBM                 0x60
#define CC1101_PA_10_DBM                0xC0

/**
 * Length of the FIFOs in bytes
 */
#define CC1101_TXFIFO_LEN        64
#define CC1101_RXFIFO_LEN        64
#define CC1101_TXFIFO_REFILL     32

/**
 * Packet length
 */
#define CC1101_PKT_FIXED_LEN      0
#define CC1101_PKT_INF_LEN        2

/**
 * Command strobes
 */
#define CC1101_SRES              0x30       // Reset CC1101 chip
#define CC1101_SFSTXON           0x31       // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1). If in RX (with CCA):
                                            // Go to a wait state where only the synthesizer is running (for quick RX / TX turnaround).
#define CC1101_SXOFF             0x32       // Turn off crystal oscillator
#define CC1101_SCAL              0x33       // Calibrate frequency synthesizer and turn it off. SCAL can be strobed from IDLE mode without
                                            // setting manual calibration mode (MCSM0.FS_AUTOCAL=0)
#define CC1101_SRX               0x34       // Enable RX. Perform calibration first if coming from IDLE and MCSM0.FS_AUTOCAL=1
#define CC1101_STX               0x35       // In IDLE state: Enable TX. Perform calibration first if MCSM0.FS_AUTOCAL=1.
                                            // If in RX state and CCA is enabled: Only go to TX if channel is clear
#define CC1101_SIDLE             0x36       // Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable
#define CC1101_SWOR              0x38       // Start automatic RX polling sequence (Wake-on-Radio) as described in Section 19.5 if
                                            // WORCTRL.RC_PD=0
#define CC1101_SPWD              0x39       // Enter power down mode when CSn goes high
#define CC1101_SFRX              0x3A       // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC1101_SFTX              0x3B       // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC1101_SWORRST           0x3C       // Reset real time clock to Event1 value
#define CC1101_SNOP              0x3D       // No operation. May be used to get access to the chip status byte

/**
 * CC1101 configuration registers
 */
#define CC1101_IOCFG2            0x00        // GDO2 Output Pin Configuration
#define CC1101_IOCFG1            0x01        // GDO1 Output Pin Configuration
#define CC1101_IOCFG0            0x02        // GDO0 Output Pin Configuration
#define CC1101_FIFOTHR           0x03        // RX FIFO and TX FIFO Thresholds
#define CC1101_SYNC1             0x04        // Sync Word, High Byte
#define CC1101_SYNC0             0x05        // Sync Word, Low Byte
#define CC1101_PKTLEN            0x06        // Packet Length
#define CC1101_PKTCTRL1          0x07        // Packet Automation Control
#define CC1101_PKTCTRL0          0x08        // Packet Automation Control
#define CC1101_ADDR              0x09        // Device Address
#define CC1101_CHANNR            0x0A        // Channel Number
#define CC1101_FSCTRL1           0x0B        // Frequency Synthesizer Control
#define CC1101_FSCTRL0           0x0C        // Frequency Synthesizer Control
#define CC1101_FREQ2             0x0D        // Frequency Control Word, High Byte
#define CC1101_FREQ1             0x0E        // Frequency Control Word, Middle Byte
#define CC1101_FREQ0             0x0F        // Frequency Control Word, Low Byte
#define CC1101_MDMCFG4           0x10        // Modem Configuration
#define CC1101_MDMCFG3           0x11        // Modem Configuration
#define CC1101_MDMCFG2           0x12        // Modem Configuration
#define CC1101_MDMCFG1           0x13        // Modem Configuration
#define CC1101_MDMCFG0           0x14        // Modem Configuration
#define CC1101_DEVIATN           0x15        // Modem Deviation Setting
#define CC1101_MCSM2             0x16        // Main Radio Control State Machine Configuration
#define CC1101_MCSM1             0x17        // Main Radio Control State Machine Configuration
#define CC1101_MCSM0             0x18        // Main Radio Control State Machine Configuration
#define CC1101_FOCCFG            0x19        // Frequency Offset Compensation Configuration
#define CC1101_BSCFG             0x1A        // Bit Synchronization Configuration
#define CC1101_AGCCTRL2          0x1B        // AGC Control
#define CC1101_AGCCTRL1          0x1C        // AGC Control
#define CC1101_AGCCTRL0          0x1D        // AGC Control
#define CC1101_WOREVT1           0x1E        // High Byte Event0 Timeout
#define CC1101_WOREVT0           0x1F        // Low Byte Event0 Timeout
#define CC1101_WORCTRL           0x20        // Wake On Radio Control
#define CC1101_FREND1            0x21        // Front End RX Configuration
#define CC1101_FREND0            0x22        // Front End TX Configuration
#define CC1101_FSCAL3            0x23        // Frequency Synthesizer Calibration
#define CC1101_FSCAL2            0x24        // Frequency Synthesizer Calibration
#define CC1101_FSCAL1            0x25        // Frequency Synthesizer Calibration
#define CC1101_FSCAL0            0x26        // Frequency Synthesizer Calibration
#define CC1101_RCCTRL1           0x27        // RC Oscillator Configuration
#define CC1101_RCCTRL0           0x28        // RC Oscillator Configuration
#define CC1101_FSTEST            0x29        // Frequency Synthesizer Calibration Control
#define CC1101_PTEST             0x2A        // Production Test
#define CC1101_AGCTEST           0x2B        // AGC Test
#define CC1101_TEST2             0x2C        // Various Test Settings
#define CC1101_TEST1             0x2D        // Various Test Settings
#define CC1101_TEST0             0x2E        // Various Test Settings

/**
 * Status registers
 */
#define CC1101_PARTNUM           0x30       // Chip ID
#define CC1101_VERSION           0x31       // Chip ID
#define CC1101_FREQEST           0x32       // Frequency Offset Estimate from Demodulator
#define CC1101_LQI               0x33       // Demodulator Estimate for Link Quality
#define CC1101_RSSI              0x34       // Received Signal Strength Indication
#define CC1101_MARCSTATE         0x35       // Main Radio Control State Machine State
#define CC1101_WORTIME1          0x36       // High Byte of WOR Time
#define CC1101_WORTIME0          0x37       // Low Byte of WOR Time
#define CC1101_PKTSTATUS         0x38       // Current GDOx Status and Packet Status
#define CC1101_VCO_VC_DAC        0x39       // Current Setting from PLL Calibration Module
#define CC1101_TXBYTES           0x3A       // Underflow and Number of Bytes
#define CC1101_RXBYTES           0x3B       // Overflow and Number of Bytes
#define CC1101_RCCTRL1_STATUS    0x3C       // Last RC Oscillator Calibration Result
#define CC1101_RCCTRL0_STATUS    0x3D       // Last RC Oscillator Calibration Result 

/**
 * CC1101 configuration registers - Can be determined with SmartRF Studio
 *
 * Configuration:
 *
 * Deviation = 380.859375
 * Base frequency = 432.999817
 * Channel spacing = 49.987793
 * Channel number = 10
 * Carrier frequency = 433.499695
 * Data rate = 999.756 Kbps
 * RX filter BW = 58.035714
 * Modulation format = 2-FSK
 * Manchester enable = false
 * Data whitening = off
 * Modulated = true
 * Sync word qualifier mode = No preamble/sync
 * Preamble count = 2
 * Data format = Normal mode
 * Length config = Infinite packet length mode
 * Address config = No address check
 * PA ramping = false
 * TX power = 10
  */
#define CC1101_DEFVAL_IOCFG2            0x29    //  GDO2 Output Pin Configuration
#define CC1101_DEFVAL_IOCFG1            0x2E    //  GDO1 Output Pin Configuration
#define CC1101_DEFVAL_IOCFG0            0x02    //  GDO0 Output Pin Configuration
#define CC1101_DEFVAL_FIFOTHR           0x08    //  RX FIFO and TX FIFO Thresholds
#define CC1101_DEFVAL_SYNC1             0xD3    //  Sync Word, High Byte
#define CC1101_DEFVAL_SYNC0             0x91    //  Sync Word, Low Byte
#define CC1101_DEFVAL_PKTLEN            0x58    //  Packet Length
#define CC1101_DEFVAL_PKTCTRL1          0x00    //  Packet Automation Control
#define CC1101_DEFVAL_PKTCTRL0          0x02    //  Packet Automation Control
#define CC1101_DEFVAL_ADDR              0x00    //  Device Address
#define CC1101_DEFVAL_CHANNR            0x0A    //  Channel Number
#define CC1101_DEFVAL_FSCTRL1           0x06    //  Frequency Synthesizer Control
#define CC1101_DEFVAL_FSCTRL0           0x00    //  Frequency Synthesizer Control
// Carrier frequency = 868 MHz
#define CC1101_DEFVAL_FREQ2_868         0x21    // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_868         0x62    // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_868         0x76    // Frequency Control Word, Low Byte
// Carrier frequency = 902 MHz
#define CC1101_DEFVAL_FREQ2_915         0x22    // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_915         0xB1    // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_915         0x3B    // Frequency Control Word, Low Byte
// Carrier frequency = 918 MHz
#define CC1101_DEFVAL_FREQ2_918         0x23    // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_918         0x4E    // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_918         0xC4    // Frequency Control Word, Low Byte
// Carrier frequency = 433 MHz
#define CC1101_DEFVAL_FREQ2_433         0x10    // Frequency Control Word, High Byte
#define CC1101_DEFVAL_FREQ1_433         0xA7    // Frequency Control Word, Middle Byte
#define CC1101_DEFVAL_FREQ0_433         0x62    // Frequency Control Word, Low Byte
// Data rate 1.001122 kBaud
#define CC1101_DEFVAL_MDMCFG4_SLOW      0xF5    // Modem Configuration
#define CC1101_DEFVAL_MDMCFG3_SLOW      0x43    // Modem Configuration
// Data rate 999.756 kBaud
#define CC1101_DEFVAL_MDMCFG4_FAST      0xFF    //  Modem Configuration
#define CC1101_DEFVAL_MDMCFG3_FAST      0x3B    //  Modem Configuration
#define CC1101_DEFVAL_MDMCFG2           0x80    //  Modem Configuration
#define CC1101_DEFVAL_MDMCFG1           0x00    //  Modem Configuration
#define CC1101_DEFVAL_MDMCFG0           0xF8    //  Modem Configuration
#define CC1101_DEFVAL_DEVIATN           0x77    //  Modem Deviation Setting
#define CC1101_DEFVAL_MCSM2             0x07    //  Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_MCSM1             0x00    //  Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_MCSM0             0x38    //  Main Radio Control State Machine Configuration
#define CC1101_DEFVAL_FOCCFG            0x16    //  Frequency Offset Compensation Configuration
#define CC1101_DEFVAL_BSCFG             0x6C    //  Bit Synchronization Configuration
#define CC1101_DEFVAL_AGCCTRL2          0x03    //  AGC Control
#define CC1101_DEFVAL_AGCCTRL1          0x40    //  AGC Control
#define CC1101_DEFVAL_AGCCTRL0          0x91    //  AGC Control
#define CC1101_DEFVAL_WOREVT1           0x87    //  High Byte Event0 Timeout
#define CC1101_DEFVAL_WOREVT0           0x6B    //  Low Byte Event0 Timeout
#define CC1101_DEFVAL_WORCTRL           0xFB    //  Wake On Radio Control
#define CC1101_DEFVAL_FREND1            0x56    //  Front End RX Configuration
#define CC1101_DEFVAL_FREND0            0x10    //  Front End TX Configuration
#define CC1101_DEFVAL_FSCAL3_SLOW       0xE9    //  Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL3_FAST       0xEA    //  Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL2            0x2A    //  Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL1            0x00    //  Frequency Synthesizer Calibration
#define CC1101_DEFVAL_FSCAL0            0x1F    //  Frequency Synthesizer Calibration
#define CC1101_DEFVAL_RCCTRL1           0x41    //  RC Oscillator Configuration
#define CC1101_DEFVAL_RCCTRL0           0x00    //  RC Oscillator Configuration
#define CC1101_DEFVAL_FSTEST            0x59    //  Frequency Synthesizer Calibration Control
#define CC1101_DEFVAL_PTEST             0x7F    //  Production Test
#define CC1101_DEFVAL_AGCTEST           0x3F    //  AGC Test
#define CC1101_DEFVAL_TEST2             0x81    //  Various Test Settings
#define CC1101_DEFVAL_TEST1             0x35    //  Various Test Settings
#define CC1101_DEFVAL_TEST0             0x09    //  Various Test Settings
#define CC1101_DEFVAL_PARTNUM           0x00    //  Chip ID
#define CC1101_DEFVAL_VERSION           0x04    //  Chip ID
#define CC1101_DEFVAL_FREQEST           0x00    //  Frequency Offset Estimate from Demodulator
#define CC1101_DEFVAL_LQI               0x00    //  Demodulator Estimate for Link Quality
#define CC1101_DEFVAL_RSSI              0x00    //  Received Signal Strength Indication
#define CC1101_DEFVAL_MARCSTATE         0x00    //  Main Radio Control State Machine State
#define CC1101_DEFVAL_WORTIME1          0x00    //  High Byte of WOR Time
#define CC1101_DEFVAL_WORTIME0          0x00    //  Low Byte of WOR Time
#define CC1101_DEFVAL_PKTSTATUS         0x00    //  Current GDOx Status and Packet Status
#define CC1101_DEFVAL_VCO_VC_DAC        0x00    //  Current Setting from PLL Calibration Module
#define CC1101_DEFVAL_TXBYTES           0x00    //  Underflow and Number of Bytes
#define CC1101_DEFVAL_RXBYTES           0x00    //  Overflow and Number of Bytes
#define CC1101_DEFVAL_RCCTRL1_STATUS    0x00    //  Last RC Oscillator Calibration Result
#define CC1101_DEFVAL_RCCTRL0_STATUS    0x00    //  Last RC Oscillator Calibration Result


//----------------------------------- TI CONSTANTS ------------------------------------------------

#define CSN_PORT PORTD
#define CSN_PIN  PIN4

#define GD2_PORT PORTD
#define GD2_PIN  PIN2

#define GD0_PORT PORTD
#define GD0_PIN  PIN3

#define SCK_PORT PORTC
#define SCK_PIN  PIN5

#define SI_PORT  PORTC
#define SI_PIN   PIN6

#define SO_PORT  PORTC //GD1
#define SO_PIN   PIN7

void cc1101_reset();
void cc1101_init(enum DATARATE rate);

inline void cc1101_select();
inline void cc1101_unSelect();
inline void cc1101_waitReady();

uint8_t cc1101_readReg(uint8_t address, uint8_t regType);
uint8_t cc1101_readSingle(uint8_t address);
uint8_t cc1101_readStatus(uint8_t address);
void cc1101_writeSingle(uint8_t address, uint8_t value);
inline void cc1101_writeBurst(uint8_t address, uint8_t *data, uint8_t len);
void cc1101_writeCmdStrobe(uint8_t command );

void cc1101_setTxPowerAmp(uint8_t level);
void cc1101_setCCregs(enum DATARATE rate);
void cc1101_setChannel(uint8_t chnl);

void cc1101_setCarrierFreq(uint8_t freq);
void cc1101_synthStartupState();
void cc1101_setTxState();
void cc1101_flushTxFifo();
void cc1101_setIdleState();
void cc1101_setSleepState(); 

void cc1101_sendDataPollGdo0(uint8_t *data, uint16_t numBytes);
void cc1101_sendDataPollTxFifo(uint8_t *data, uint16_t numBytes);

extern uint8_t cc1101_crc;
extern uint8_t cc1101_lqi;
