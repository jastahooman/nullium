#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

extern uint8_t inb(uint16_t port);


void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
uint32_t inl(uint16_t port);
void outl(uint16_t port, uint32_t value);

extern void io_wait(void);
