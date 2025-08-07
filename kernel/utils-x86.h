#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);
extern void outw(uint16_t port, uint16_t val);

extern void io_wait(void);
