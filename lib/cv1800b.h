#ifndef CV1800B_H
#define CV1800B_H

#include <stdint.h>

typedef struct gpio_registers{
    volatile uint32_t SWPORTA_DR;
    volatile uint32_t SWPORTA_DDR;
    volatile uint32_t __RESERVED1[10];
    volatile uint32_t INTEN;
    volatile uint32_t INTMASK;
    volatile uint32_t INTTYPE_LEVEL;
    volatile uint32_t INT_POLARITY;
    volatile uint32_t INTSTATUS;
    volatile uint32_t RAW_INTSTATUS;
    volatile uint32_t DEBOUNCE;
    volatile uint32_t PORTA_EOI;
    volatile uint32_t EXT_PORTA;
    volatile uint32_t __RESERVED2[3];
    volatile uint32_t LS_SYNC;
} gpio_registers;

#define GPIO_BASE_ADDRESS (0x03020000)
#define GPIO0 ((gpio_registers *)(GPIO_BASE_ADDRESS + 0x0000))
#define GPIO1 ((gpio_registers *)(GPIO_BASE_ADDRESS + 0x1000))
#define GPIO2 ((gpio_registers *)(GPIO_BASE_ADDRESS + 0x2000))
#define GPIO3 ((gpio_registers *)(GPIO_BASE_ADDRESS + 0x3000))

#endif // CV1800B_H