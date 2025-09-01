/*
    PopKernel
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




void crash(const char* str){


    for(unsigned int y = 0; y < gfx_resY; y += 2){
        for(unsigned int x = 0; x < gfx_resX; x += 2){
            gfx_plotPixel(x + 1, y, 0x000000);
        }
        for(unsigned int x = 0; x < gfx_resX; x += 2){
            gfx_plotPixel(x,  y + 1, 0x000000);
        }
    }

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
    
    
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

extern int stage3_boot();

#include <stage3/nterm.h>

terminal_char_t default_textmode_fb[90 * 40]; // initial framebuffer, a better one will be initialized later at boot


void stage2_boot(void){


    //gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x5500AA);
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x444466);    //0x444466 

    terminal_init((terminal_char_t*)&default_textmode_fb, 90, 40, (gfx_resX / 2) - (((font_width + 1) * 90)/2), (gfx_resY / 2) - (((font_height + 1) * 40)/2));
    
    terminal_print("Welcome to PopKernel!\n\n", 0, 14);
    

    init_GDT();
    terminal_print("GDT init ... OK.\n", 0, 15);
    

    init_IDT();
    terminal_print("IDT init ... OK.\n", 0, 15);
    

        
    PIT_Init();
    terminal_print("PIT init ... OK.\n", 0, 15);



    

    PS2_Init();
    terminal_print("PS/2 keyboard init ... OK.\n", 0, 15);

    sleep(50);



    pmm_Init();
    terminal_print("PMM init ... OK.\n", 0, 15);
    

    init_Paging();
    
    terminal_print("Paging init ... OK.\n", 0, 15);
    
    //init_vmm();
    //terminal_print("VMM ... OK.\n", 0, 15);


    for(;;);

    sleep(30);




    sleep(60);
    uint32_t y = 5;
    y += (font_height + 5) * 2;

    sleep(60);
    stage3_boot();


    
}