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

// Timer driver

#include <stdint.h>
#include <utils/utils-x86.h>
#include <utils/utils.h>
#include <drivers/idt.h>
#include <drivers/graphics.h>
#include <drivers/lv1io.h>

uint64_t ticks;
uint64_t ticks_NoReset;

const uint32_t frequency = 100;

void Timer_Reset(void){
    ticks = 0;
}

extern void s3_tick(void);
extern bool loaded3;
void PIT_Tick(struct InterruptRegisters *regs){
    ticks += 1;
    ticks_NoReset =+ 1;

    if (loaded3){
        s3_tick();
    }
}

void PIT_Init(void){
    ticks = 0;
    IRQ_setHandler(0, &PIT_Tick);

    //119318.16666 Mhz
    uint32_t divisor = 1193180/frequency;

    //0011 0110
    outb(0x43,0x36);
    outb(0x40,(uint8_t)(divisor & 0xFF));
    outb(0x40,(uint8_t)((divisor >> 8) & 0xFF));
}
