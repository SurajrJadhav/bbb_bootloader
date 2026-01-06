#include <stdint.h>
#include "blink.h"
#include "uart.h"

void main(void)
{
    // Initialize UART0 for debug output
    uart_init();
    
    // Send startup message
    uart_puts("\n\n");
    uart_puts("========================================\n");
    uart_puts("BeagleBone Black Bootloader Started!\n");
    uart_puts("UART0 @ 115200 baud, 8N1\n");
    uart_puts("========================================\n");
    uart_puts("\n");
    
    usr0_led_init();
    
    uart_puts("LED initialization complete.\n");
    uart_puts("Starting LED blink sequence...\n\n");
    
    int count = 0;
    while (1) {
        // Turn LED ON
        uart_puts("LED ON  [");
        uart_putc('0' + (count % 10));
        uart_puts("]\n");
        usr0_led_on();
        for(int i=0; i<5000000; i++);
        
        // Turn LED OFF
        uart_puts("LED OFF [");
        uart_putc('0' + (count % 10));
        uart_puts("]\n");
        usr0_led_off();
        for(int i=0; i<5000000; i++);
        
        count++;
    }
}
