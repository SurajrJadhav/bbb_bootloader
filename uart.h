#ifndef UART_H
#define UART_H

#include <stdint.h>

// ====================================================================
// UART0 Base Address and Register Offsets
// ====================================================================
#define UART0_BASE              0x44E09000

// UART Register Offsets
#define UART_THR                0x00    // Transmit Holding Register (Write, when LCR[7]=0)
#define UART_RHR                0x00    // Receive Holding Register (Read, when LCR[7]=0)
#define UART_IER                0x04    // Interrupt Enable Register (when LCR[7]=0)
#define UART_FCR                0x08    // FIFO Control Register (Write Only)
#define UART_LCR                0x0C    // Line Control Register
#define UART_MCR                0x10    // Modem Control Register
#define UART_LSR                0x14    // Line Status Register
#define UART_MSR                0x18    // Modem Status Register
#define UART_SPR                0x1C    // Scratchpad Register
#define UART_MDR1               0x20    // Mode Definition Register 1
#define UART_SYSC               0x54    // System Configuration Register
#define UART_SYSS               0x58    // System Status Register

// DLL and DLH are accessible when LCR[7] = 1 (Divisor Latch Access Bit)
#define UART_DLL                0x00    // Divisor Latch Low
#define UART_DLH                0x04    // Divisor Latch High

// ====================================================================
// Clock Control for UART0
// ====================================================================
#define CM_WKUP_BASE            0x44E00400
#define CM_WKUP_UART0_CLKCTRL   0xB4    // Offset from CM_WKUP_BASE

// ====================================================================
// Pin Mux Configuration for UART0
// ====================================================================
#define CONTROL_MODULE_BASE     0x44E10000
#define CONF_UART0_RXD          0x970   // Offset for UART0 RX pin mux
#define CONF_UART0_TXD          0x974   // Offset for UART0 TX pin mux

// Pin Mux Values
// Mode 0 = UART0, Bit 5 = RX Enable (1 for RX, 0 for TX), Bit 4:3 = Pull (01 = pull-up)
#define UART0_RXD_MUX_VALUE     0x30    // Mode 0, RX enabled, pull-up
#define UART0_TXD_MUX_VALUE     0x00    // Mode 0, TX enabled, no pull

// ====================================================================
// UART Configuration Values
// ====================================================================
// Line Control Register (LCR) bits
#define UART_LCR_8N1            0x03    // 8 data bits, No parity, 1 stop bit
#define UART_LCR_DLAB           0x80    // Divisor Latch Access Bit

// Line Status Register (LSR) bits
#define UART_LSR_THRE           0x20    // Transmit Holding Register Empty
#define UART_LSR_TEMT           0x40    // Transmitter Empty

// FIFO Control Register (FCR) bits
#define UART_FCR_FIFO_EN        0x01    // Enable FIFO
#define UART_FCR_RXCLR          0x02    // Clear RX FIFO
#define UART_FCR_TXCLR          0x04    // Clear TX FIFO

// Mode Definition Register 1 (MDR1) values
#define UART_MDR1_MODE_DISABLE  0x07    // UART disabled
#define UART_MDR1_MODE_16X      0x00    // 16x mode (standard UART)

// System Status Register (SYSS) bits
#define UART_SYSS_RESETDONE     0x01    // Reset done

// ====================================================================
// Baud Rate Calculation
// ====================================================================
// UART0 functional clock = 48 MHz
// Baud Rate = Clock / (16 * Divisor)
// For 115200 baud: Divisor = 48000000 / (16 * 115200) = 26.041... ≈ 26
#define UART_BAUD_115200_DLL    26      // Divisor for 115200 baud
#define UART_BAUD_115200_DLH    0

// ====================================================================
// Function Prototypes
// ====================================================================
void uart_init(void);
void uart_putc(char c);
void uart_puts(const char *str);

#endif // UART_H
