/*
    The Nullium Driver Software Development Kit

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


#include "../include/drivers.h"

#include <stdbool.h>
#include <stdint.h>

uint32_t get_edi(){
    uint32_t EDI;
    asm( "movl %%edi, %0"
    : "=a" (EDI)
    );

    return EDI;
}

void systemcall(uint32_t eax, uint32_t ecx, uint32_t edx, uint32_t esi){

    __asm__ volatile( "movl %0, %%eax"
    :
    : "b" (eax)
    : "eax"
    );


    __asm__ volatile( "movl %0, %%esi"
    :
    : "b" (esi)
    : "esi"
    );

    __asm__ volatile( "movl %0, %%ecx"
    :
    : "b" (ecx)
    : "ecx"
    );

    __asm__ volatile( "movl %0, %%edx"
    :
    : "b" (edx)
    : "edx"
    );

    asm("int $0x45");
}
uint32_t deviceid;

void dsdk_setup(uint16_t devicetype){
    //systemcall(2, devicetype, 0, 0);
    deviceid = get_edi();
}
