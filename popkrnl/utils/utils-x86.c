/*
    The Nullium Operating System
    Copyright (C) 2025, jastahooman

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
};

void outw(uint16_t port, uint16_t val)
{
    __asm__ volatile ( "outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
};

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}




uint32_t inl(uint16_t port) {
    uint32_t result;
    __asm__ volatile (
        "inl %[port], %[result]"  
        : [result] "=a" (result)
        : [port] "d" (port)     
    );
    return result;
}

void outl(uint16_t port, uint32_t value) {
    __asm__ volatile (
        "outl %0, %1"  
        :
        : "a" (value), "Nd" (port)  
    );
}

void io_wait(void){
    outb(0x80, 0);
}
