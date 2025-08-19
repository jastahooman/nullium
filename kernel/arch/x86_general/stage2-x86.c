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
#include <utils/utils.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <drivers/lv1io.h>
#include <drivers/graphics.h>
#include "krnlBitmaps.h"
#include <stdio.h>
#include <drivers/keyboard.h>
#include <utils/utils-x86.h>
#include <drivers/lv2io.h>

#include <drivers/ldata.h>

long gfx_resX;
long gfx_resY;
long gfx_bpp;

struct os_bootParams bootParams;




void extCrash(const char* str){
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x222222);
    uint32_t y = 5;
    putstr("Panic!", 5, y, 0xFFFFFF);
    y += (font_height + 5) * 2;

    putstr("A fatal system error occured. Please restart your PC manually.", 5, y, 0xFFFFFF);
    y += (font_height + 5) * 2;
    putstr("System Info:", 5, y, 0xFFFFFF);
    y += (font_height + 5) * 1;
    putstr("PC Type:", 5, y, 0xFFFFFF);
    putstr(PCtype, 5 + (25 * (font_width + 1)), y, 0xFFFFFF);

    y += (font_height + 5) * 1;
    putstr("CPU Architecture:", 5, y, 0xFFFFFF);
    putstr(CPUArch, 5 + (25 * (font_width + 1)), y, 0xFFFFFF);

    y += (font_height + 5) * 1;
    putstr("Boot Protocol:", 5, y, 0xFFFFFF);
    putstr(BootProtocol, 5 + (25 * (font_width + 1)), y, 0xFFFFFF);



    y += (font_height + 5) * 1;
    putstr("Firmware:", 5, y, 0xFFFFFF);
    putstr(PCfirmware, 5 + (25 * (font_width + 1)), y, 0xFFFFFF);


    y += (font_height + 5) * 2;
    putstr("OS version:\n    Nullium\n    version 1.0 Update 0", 5, y, 0xFFFFFF);
    
    y += (font_height + 5) * 2;
    
    putstr("OS links, in case you want to report or etc:\nGitHub: https://github.com/jastahooman/nullium\nWebsite: https://example.com/", 5, gfx_resY - ((font_height + 5)*3), 0xFFFFFF);
    
    y += (font_height + 5) * 2;
    putstr("Crash Reason: ", 5, y, 0xFFFFFF);
    putstr(str, 5 + (14 * (font_width + 1)), y, 0xFFFFFF);
    
    
    y += (font_height + 5) * 2;
    putstr("Task Dump: ", 5, y, 0xFFFFFF);
    
    y += (font_height + 5) * 1;
    putstr("Task:", 5, y, 0xFFFF00);
    putstr("Starts at:", 150, y, 0xFFFF00);
    putstr("Ends at:", 300, y, 0xFFFF00);


    char* mem = "0xFFFFFFFFFFFFFFFF";

    y += (font_height + 5) * 1;
    putstr("Kernel", 5, y, 0xFFFFFF);
    putstr("0x", 150, y, 0xFFFFFF);
    putstr(itoa((int)&ld_start, mem, 16), 150 + ((font_width + 1) * 2), y, 0xFFFFFF);
    
    putstr("0x", 300, y, 0xFFFFFF);
    putstr(itoa((int)&ld_end, mem, 16), 300 + ((font_width + 1) * 2), y, 0xFFFFFF);
    


    y += (font_height + 5) * 2;

    asm ("cli");
    for (;;) {
        asm ("hlt");
    }

}

void crash(const char* str){

    if (bootParams.extCrashData){
        extCrash(str);
    }
    
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
    putstr("System architecture: ", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 5) * 1), 0x000000);
    putstr(PCtype, (gfx_resX / 4) + 5 + ((font_width + 1) * 0), (gfx_resY / 4) + 5 + ((font_height + 5) * 2), 0x000000);
    putstr(CPUArch, (gfx_resX / 4) + 5 + ((font_width + 1) * 0), (gfx_resY / 4) + 5 + ((font_height + 5) * 3), 0x000000);

    putstr("OS version:\n    Nullium\n    version 1.0 Update 0", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 5) * 5), 0x000000);
    

    putstr("Crash Reason: ", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 5) * 9), 0x000000);
    putstr(str, (gfx_resX / 4) + 5 + ((font_width + 1) * 14), (gfx_resY / 4) + 5 + ((font_height + 5) * 9), 0x000000);

    putstr("OS Links:\nGitHub: https://github.com/jastahooman/nullium", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 5) * 11), 0x000000);
    putstr("Web site: https://example.com", (gfx_resX / 4) + 5, (gfx_resY / 4) + 5 + ((font_height + 5) * 13), 0x000000);
    

    
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

extern int stage3_boot(struct os_bootParams bootConf);

void stage2_boot(void){
    bootParams.extCrashData = true;
    //gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x5500AA);
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x0055AA);

    unsigned int idx = 0;
    for(unsigned int y = 0; y < 12; y++){

        for(unsigned int x = 0; x < 12; x++){
            if (menuBtn[idx] == 1){
                gfx_plotPixel(x + (gfx_resX/2) - 12 , y + (gfx_resY/2) - 12, 0x000000);
            }
            idx++;
        }
    }


    init_GDT();

    
    init_IDT();

    
    PIT_Init();

    sleep(10);

    sleep(5);
    PS2_Init();

    init_mem();
    init_paging();

    sleep(30);

    bootParams.extCrashData = false;
    idx = 0;
    for(unsigned int y = 0; y < 12; y++){

        for(unsigned int x = 0; x < 12; x++){
            if (menuBtn[idx] == 1){
                gfx_plotPixel(x + (gfx_resX/2) - 12 , y + (gfx_resY/2) - 12, 0xFFFFFF);
            }
            idx++;
        }
    }

    sleep(60);

    bootParams.disableSysExt = false;
    bootParams.extCrashData = false;

    uint32_t y = 5;
    y += (font_height + 5) * 2;

    if (kb_detectPress('c')){
        bootParams.extCrashData = true;
        shadowTxt("Extended crash info.", 5, y, 0xFFFFFF, 0x000000);
        y += font_height + 5;
    }
    if (kb_detectPress('e')){
        bootParams.disableSysExt = true;
        shadowTxt("Extensions off...", 5, y, 0xFFFFFF, 0x000000);
        y += font_height + 5;
    }

    sleep(60);
    stage3_boot(bootParams);


    
}