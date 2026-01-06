#include "blink.h"

// --- Utility Functions (Simplified for early boot) ---
static void delay_loop(unsigned long count)
{
    // Simple, uncalibrated delay loop for the MLO
    while (count--);
}

// ====================================================================
void usr0_led_init(void){
        // Enable GPIO1 clock for LED
    *(volatile unsigned int *)(CM_PER_GPIO1_CLKCTRL) = 0x2;
    
    // Configure Pin Mux for USR0 (GPIO1_21)
    *(volatile unsigned int *)(CONF_GPMC_AD9) = USR0_MUX_VALUE;
    
    // Set GPIO1_21 as OUTPUT
    *(volatile unsigned int *)(GPIO1_BASE + GPIO_OE) &= ~USR0_PIN;
}

void usr0_led_on(){
    *(volatile unsigned int *)(GPIO1_BASE + GPIO_SETDATAOUT) = USR0_PIN;
}

void usr0_led_off(){
    *(volatile unsigned int *)(GPIO1_BASE + GPIO_CLEARDATAOUT) = USR0_PIN;
}

void usr0_led_blink_continuesly(void)
{
    // 4. Blink the LED (e.g., 5 times)
    for (;;) {
        // TURN LED ON (Set Data Out Register)
        // Address: 0x4804C194
        *(volatile unsigned int *)(GPIO1_BASE + GPIO_SETDATAOUT) = USR0_PIN;
        delay_loop(1000000); // Wait ON

        // TURN LED OFF (Clear Data Out Register)
        // Address: 0x4804C190
        *(volatile unsigned int *)(GPIO1_BASE + GPIO_CLEARDATAOUT) = USR0_PIN;
        delay_loop(1000000); // Wait OFF
    }

    // Leave the LED off after the blink sequence
}
