#ifndef BLINK_H_
#define BLINK_H_

#define CM_PER_BASE         0x44E00000
#define CM_PER_GPIO1_CLKCTRL (CM_PER_BASE + 0xAC)

#define GPIO1_BASE          0x4804C000
#define GPIO_OE             0x134  // Output Enable Register
#define GPIO_SETDATAOUT     0x194  // Set Output Data Register (Set Pin HIGH)
#define GPIO_CLEARDATAOUT   0x190  // Clear Output Data Register (Set Pin LOW)

#define CONTROL_MODULE_BASE 0x44E10000
// Mux register for GPIO1_21. Check TRM for exact value for mode 7 (GPIO).
#define CONF_GPMC_AD9       (CONTROL_MODULE_BASE + 0x824)

// --- USR0 LED Specifics ---
#define USR0_PIN            (1 << 21) // Pin 21 of GPIO Bank 1
#define USR0_MUX_VALUE      (7)       // Mux mode 7 for GPIO, pull-up/down disabled

static void delay_loop(unsigned long count);
void usr0_led_blink_continuesly();
void usr0_led_init();
void usr0_led_on();
void usr0_led_off();

#endif // BLINK_H_
