#include "uart.h"

// ====================================================================
// Helper Macros for Register Access
// ====================================================================
#define REG32(addr) (*(volatile uint32_t *)(addr))

// ====================================================================
// UART Initialization Function
// ====================================================================
void uart_init(void)
{
    // Step 1: Enable UART0 Module Clock
    // Write 0x2 to enable the functional clock for UART0
    REG32(CM_WKUP_BASE + CM_WKUP_UART0_CLKCTRL) = 0x2;
    
    // Wait for clock to be enabled (optional: check IDLEST bits)
    // For simplicity in bootloader, we'll add a small delay
    for(volatile int i = 0; i < 1000; i++);
    
    // Step 2: Configure Pin Mux for UART0 TX and RX
    // UART0_RXD: Mode 0, RX enabled, pull-up
    REG32(CONTROL_MODULE_BASE + CONF_UART0_RXD) = UART0_RXD_MUX_VALUE;
    
    // UART0_TXD: Mode 0, TX enabled
    REG32(CONTROL_MODULE_BASE + CONF_UART0_TXD) = UART0_TXD_MUX_VALUE;
    
    // // Step 3: Perform Soft Reset (Optional but recommended)
    // // Set SOFTRESET bit in SYSC register
    // REG32(UART0_BASE + UART_SYSC) = 0x2;
    
    // // Wait for reset to complete by checking SYSS register
    // while ((REG32(UART0_BASE + UART_SYSS) & UART_SYSS_RESETDONE) == 0);
    
    // Step 4: Disable UART (set to disabled mode)
    REG32(UART0_BASE + UART_MDR1) = UART_MDR1_MODE_DISABLE;
    
    // Step 5: Set Divisor Latch Access Bit (DLAB) to access DLL/DLH
    REG32(UART0_BASE + UART_LCR) = UART_LCR_DLAB;
    
    // Step 6: Set Baud Rate Divisor for 115200 baud
    REG32(UART0_BASE + UART_DLL) = UART_BAUD_115200_DLL;
    REG32(UART0_BASE + UART_DLH) = UART_BAUD_115200_DLH;
    
    // Step 7: Clear DLAB and set 8N1 format (8 data bits, No parity, 1 stop bit)
    REG32(UART0_BASE + UART_LCR) = UART_LCR_8N1;
    
    // Step 8: Enable and Clear FIFOs
    REG32(UART0_BASE + UART_FCR) = UART_FCR_FIFO_EN | UART_FCR_RXCLR | UART_FCR_TXCLR;
    
    // Step 9: Enable UART in 16x mode (standard UART mode)
    REG32(UART0_BASE + UART_MDR1) = UART_MDR1_MODE_16X;
    
    // UART0 is now ready to transmit and receive!
}

// ====================================================================
// UART Character Transmit Function
// ====================================================================
void uart_putc(char c)
{
    // Wait until Transmit Holding Register is Empty (THRE bit is set)
    while ((REG32(UART0_BASE + UART_LSR) & UART_LSR_THRE) == 0);
    
    // Write character to Transmit Holding Register
    REG32(UART0_BASE + UART_THR) = c;
}

// ====================================================================
// UART String Transmit Function
// ====================================================================
void uart_puts(const char *str)
{
    while (*str) {
        // Handle newline: send CR+LF for proper terminal display
        if (*str == '\n') {
            uart_putc('\r');
        }
        uart_putc(*str++);
    }
}
