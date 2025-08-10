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
#include <utils/utils.h>
#include <krnlBitmaps.h>
#include <drivers/graphics.h>

void gfx_wireRect(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color){
    uint64_t x = px;
    uint64_t y = py;

    for(x = px; x < width - 1 + px; x++){
        gfx_plotPixel(x, y, color);
    }
    y++;
    for(y = py; y < height - 1 + py; y++){
        x = px;
        gfx_plotPixel(x, y, color);
        for(x = px; x < width - 1 + px; x++){
            
        }
        gfx_plotPixel(x, y, color);
    }
    for(x = px; x < width + px; x++){
            gfx_plotPixel(x, y, color);
    }
}

void gfx_putRect(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color){
    uint64_t x = px;
    uint64_t y = py;
    for(y = py; y < height + py; y++){
        for(x = px; x < width + px; x++){
            gfx_plotPixel(x, y, color);
        }
    }
}


void bufferRct(uint64_t px, uint64_t py, uint64_t width, uint64_t height){
    uint64_t x = px;
    uint64_t y = py;
    for(y = py; y < height + py; y++){
        for(x = px; x < width + px; x++){
            gfx_plotPixelD(x, y, gfx_getPixel(x, y));
        }
    }
}

void drawCurs(uint64_t px, uint64_t py, uint8_t cursor){
    unsigned int idx = 0;

    for(unsigned int y = 0; y < cursor_height; y++){

        for(unsigned int x = 0; x < cursor_width; x++){

            
                
            if (cursors[cursor][idx] == 1){
                gfx_plotPixelD(x + px, y + py, 0xFFFFFF);

            } else if (cursors[cursor][idx] == 2){
                gfx_plotPixelD(x + px, y + py, 0x000000);
            }
            idx++;
        }
    }
}

void putltr(uint64_t px, uint64_t py, uint32_t color, int letter){



    if (letter == 34 || letter == 43 || letter == 44 || letter == 52){
        py += 4;
    }
    if (letter == 37){
        py += 1;
    }

    unsigned int idx = 0;
    for(unsigned int y = 0; y < font_height; y++){

        for(unsigned int x = 0; x < font_width; x++){

            
                
            if (font_regular[letter][idx]){
                gfx_plotPixel(x + px, y + py, color);
            }
            idx++;
        }
    }
}


void putstr(const char* str, uint64_t px, uint64_t py, uint32_t color){

    uint64_t x = px;
    uint64_t y = py;
    uint64_t idx = 0;
    uint64_t searchidx = 0;

    for(idx = 0; idx < getstrsz(str); idx++){
            switch (str[idx]){

            case '\n':
                py += font_height + 4;
                x = px;
                y = py;

            break;

            default:

            for(searchidx = 0; searchidx < 100; searchidx++){
                if (fontMap[searchidx] == str[idx]){
                    break;
                }
            }
            putltr(x, y, color, searchidx);
            x += font_width + 1;

            break;
        }
    }
}

void shadowTxt(const char* str, uint64_t px, uint64_t py, uint32_t color, uint32_t shadowClr){
    putstr(str, px + 1, py + 1, shadowClr);
    putstr(str, px, py, color);
}