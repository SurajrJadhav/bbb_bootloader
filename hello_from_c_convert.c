#define UART0_BASE 0x44E09000
#define UART_THR   0x00
#define UART_LSR   0x14
#define UART_LSR_THRE (1 << 5)

static inline void uart_putc(char c)
{
    volatile unsigned int *lsr =
        (unsigned int *)(UART0_BASE + UART_LSR);
    volatile unsigned int *thr =
        (unsigned int *)(UART0_BASE + UART_THR);

    while (!(*lsr & UART_LSR_THRE));
    *thr = c;
}

void main(void)
{
    const char *msg = "HELLO BBB FROM BARE METAL\n";
    for(int i=0;i<50;i++){
	char *ptr = msg;
    	while (*ptr)
        	uart_putc(*ptr++);
    }
    while (1);
}
