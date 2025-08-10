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

// Stage 2 boot for x86 processors.

#include "st2Boot-x86.h"

#include <stage3/winmgr.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "lv1io.h"
#include <drivers/graphics.h>
#include "krnlBitmaps.h"
#include <stdio.h>
#include <drivers/keyboard.h>
#include <utils/utils-x86.h>

long gfx_resX;
long gfx_resY;
long gfx_bpp;


void crash(const char* str){
    
    for(unsigned int y = 0; y < gfx_resY; y += 2){
        for(unsigned int x = 0; x < gfx_resX; x += 2){
            gfx_plotPixel(x + 1, y, 0x000000);
        }
        for(unsigned int x = 0; x < gfx_resX; x += 2){
            gfx_plotPixel(x,  y + 1, 0x000000);
        }
    }


    
    gfx_putRect(gfx_resX / 4 - 1, gfx_resY / 4 - 1, gfx_resX / 2 + 2, gfx_resY / 2 + 2 , 0x000000);
    gfx_putRect(gfx_resX / 4, gfx_resY / 4, gfx_resX / 2, gfx_resY / 2 , 0xCCCCCC);
    putstr("Panic!", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5, 0x000000);
    putstr("Bootloader used: ", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 4) * 1), 0x000000);
    putstr(bootLdrName, (gfx_resX / 4) + 5 + ((font_width + 1) * 17), (gfx_resY / 4) + 5 + ((font_height + 4) * 1), 0x000000);
    putstr("CPU Architecture: ", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 4) * 2), 0x000000);
    putstr(CPUArch, (gfx_resX / 4) + 5 + ((font_width + 1) * 18), (gfx_resY / 4) + 5 + ((font_height + 4) * 2), 0x000000);

    putstr("Operating System version:", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 4) * 4), 0x000000);
    putstr("    Nullium\n    version 1.0 Update 0", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 4) * 5), 0x000000);

    putstr("Crash Reason: ", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 4) * 9), 0x000000);
    putstr(str, (gfx_resX / 4) + 5 + ((font_width + 1) * 0), (gfx_resY / 4) + 5 + ((font_height + 4) * 10), 0x000000);

    for (;;) {
        asm ("hlt");
    }
}

extern int stage3_boot();

void stage2_boot(void){

    //gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x5500AA);
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x0055AA);



    init_GDT();

    putstr("GDT init ... OK :^P", 2, 2, 0x2255CC); // shhhhh :^)
    
    init_IDT();

    
    PIT_Init();

    sleep(10);

    sleep(5);
    PS2_Init();


    sleep(30);

    unsigned int idx = 0;
    for(unsigned int y = 0; y < 12; y++){

        for(unsigned int x = 0; x < 12; x++){
            if (menuBtn[idx] == 1){
                gfx_plotPixel(x + (gfx_resX/2) - 12 , y + (gfx_resY/2) - 12, 0xFFFFFF);
            }
            idx++;
        }
    }


    sleep(70);

    stage3_boot();


    
}