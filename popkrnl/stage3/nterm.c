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

#include <stdint.h>
#include <stdbool.h>
#include <drivers/graphics.h>
#include <krnlBitmaps.h>
#include <drivers/lv2io.h>

struct terminal_char{
    char ltr;
    uint8_t bgcolor;
    uint8_t fgcolor;
};

typedef struct terminal_char terminal_char_t;


uint16_t terminal_width;
uint16_t terminal_height;
uint16_t terminal_offsetX;
uint16_t terminal_offsetY;

struct terminal_char* framebuffer;
/*
uint32_t trm_colors[16] = {
    0x000000, // black/bg
    0x0000AA, // dark blue
    0x00AA00, // dark green
    0x00AAAA, // cyan
    0xAA0000, // red
    0xAA00AA, // magenta
    0xAA5500, // brown
    0xAAAAAA, // light gray
    0x555555, // dark gray
    0x5555FF, // bright blue
    0x55FF55, // bright green
    0x55FFFF, // bright cyan
    0xFF5555, // bright red
    0xFF55FF, // bright magenta
    0xFFFF55, // yellow
    0xFFFFFF, // white
}; // VGA palette
*/

uint32_t trm_colors[16] = {
    0x444466, // black/bg - (indigo)
    0x4466AA, // dark blue
    0x33AA55, // dark green
    0x00BABA, // cyan
    0xAA3322, // red
    0xAD11AD, // magenta
    0xAA5500, // brown
    0xBBBBEE, // light gray
    0x7777AA, // dark gray
    0x6699FF, // bright blue
    0x55FF33, // bright green
    0x55FFFF, // bright cyan
    0xFF5522, // bright red
    0xFF55FF, // bright magenta
    0xFFFF55, // yellow
    0xFFFFFF, // white
}; 



typedef struct terminal_char terminal_char_t;

void renderTermBlit(uint32_t y){
    uint32_t x;
    uint32_t idx = (y * terminal_width);

    for (x = 0; x < terminal_width; x++){
        gfx_putRect(x * (font_width + 1) + terminal_offsetX, y * (font_height + 1) + terminal_offsetY, font_width + 1, font_height + 1, trm_colors[framebuffer[idx].bgcolor]);
        putltr(x * (font_width + 1) + terminal_offsetX, y * (font_height + 1) + terminal_offsetY, trm_colors[framebuffer[idx].fgcolor], ASCIItoPK(framebuffer[idx].ltr));
        idx++;
    }
}


void rmRow(uint32_t y){
    uint32_t x;
    uint32_t idx = (y * terminal_width);

    for (x = 0; x < terminal_width; x++){
        framebuffer[idx].ltr = ' ';
        framebuffer[idx].fgcolor = 15;
        framebuffer[idx].bgcolor = 0;
        idx++;
    }
}



void renderTerm(){
    uint32_t x;
    uint32_t y;
    uint32_t idx = 0;
    for (y = 0; y < terminal_height; y++){
        for (x = 0; x < terminal_width; x++){
            gfx_putRect(x * (font_width + 1) + terminal_offsetX, y * (font_height + 1) + terminal_offsetY, font_width + 1, font_height + 1, trm_colors[framebuffer[idx].bgcolor]);
            putltr(x * (font_width + 1) + terminal_offsetX, y * (font_height + 1) + terminal_offsetY, trm_colors[framebuffer[idx].fgcolor], ASCIItoPK(framebuffer[idx].ltr));
            idx++;
        }
    }
}

void terminal_putChar(char ltr, uint16_t row, uint16_t col, uint8_t colorBG, uint8_t colorFG){
    framebuffer[(row * terminal_width) + col].ltr = ltr;
    framebuffer[(row * terminal_width) + col].bgcolor = colorBG;
    framebuffer[(row * terminal_width) + col].fgcolor = colorFG;

    renderTermBlit(row);

}
uint16_t print_row;
uint16_t print_col;

#include <utils/utils.h>

void terminal_scrollUp(){


    uint16_t mvx = 0;
    uint16_t mvy = 0;

    for(mvy = 0; mvy < terminal_height - 1; mvy++){
        for (mvx = 0; mvx < terminal_width; mvx++){
            framebuffer[(mvy * terminal_width) + mvx].bgcolor = framebuffer[((mvy+1) * terminal_width) + mvx].bgcolor;
            framebuffer[(mvy * terminal_width) + mvx].ltr = framebuffer[((mvy+1) * terminal_width) + mvx].ltr;
            framebuffer[(mvy * terminal_width) + mvx].fgcolor = framebuffer[((mvy+1) * terminal_width) + mvx].fgcolor;
        }
    }

    rmRow(terminal_height - 1);
    renderTerm();
}

void terminal_writeStr(const char* str, uint16_t row, uint16_t col, uint8_t colorBG, uint8_t colorFG){
    
    uint32_t stringlen = strlen(str);
    uint32_t idx = 0;

    print_row = row;
    print_col = col;
    
    if (print_row > terminal_height){
        idx = print_row - terminal_height;
        print_row = terminal_height;
        for (uint32_t idx2 = 0; idx2 < idx; idx2++){
            terminal_scrollUp();
        }
        
    }

    idx = 0;

    for(idx = 0; idx < stringlen; idx++){


    switch (str[idx]){

        case '\n':
            renderTermBlit(print_row);
            print_col = 0;
            print_row++;

            

            if(print_row >= terminal_height){
                print_row-=1;
                terminal_scrollUp();
            }

            break;

            default:

            framebuffer[(print_row * terminal_width) + print_col].ltr = str[idx];
            framebuffer[(print_row * terminal_width) + print_col].bgcolor = colorBG;
            framebuffer[(print_row * terminal_width) + print_col].fgcolor = colorFG;
            print_col++;

            if(print_col > terminal_width){
                print_col = 0;
                print_row++;
            }

            if (print_row >= terminal_height){
                print_row-=1;
                terminal_scrollUp();
            }
            
                

            break;
        }
    }

    
    renderTermBlit(print_row);

}

void terminal_print(const char* str, uint8_t colorBG, uint8_t colorFG){

    terminal_writeStr(str, print_row, print_col, colorBG, colorFG);
}


void terminal_init(terminal_char_t* fb, uint16_t width, uint16_t height, uint16_t offsetX, uint16_t offsetY){
    framebuffer = fb;
    terminal_width = width;
    terminal_height = height;
    terminal_offsetX = offsetX;
    terminal_offsetY = offsetY;

    renderTerm();
}
