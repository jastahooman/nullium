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
#include <drivers/idt.h>
#include <stdint.h>
#include <stdbool.h>
#include <drivers/graphics.h>
#include <drivers/driversdkmnt-x86.h>

#define SYSCALL_EXIT 1
#define SYSCALL_DRIVER_SETUP 2

void syscall_handler(struct InterruptRegisters* regs){
    switch (regs->eax){
        case SYSCALL_DRIVER_SETUP:
            switch (regs->ecx){
                case DRIVER_SDK_DEVICETYPE_KEYBOARD:
                    putstr("Keyboard", 5, 5, 0xFFFFFF);
                break;
            }
        break;
    }
    
}