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
#include <utils/utils.h>
#include <krnlBitmaps.h>
#include <drivers/graphics.h>
#include <drivers/lv1io.h>
#include <drivers/lv2io.h>

extern uint32_t cursor_posX;
extern uint32_t cursor_posY;

extern uint32_t cursor_posX_p;
extern uint32_t cursor_posY_p;

uint32_t gfx_getPixel(uint64_t x, uint64_t y){
    return gfx_getPixelD(x, y);
}

void gfx_putRectD(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color){
    uint64_t x = px;
    uint64_t y = py;
    for(y = py; y < height + py; y++){
        for(x = px; x < width + px; x++){
            gfx_plotPixelD(x, y, color);
        }
    }
}

void gfx_plotPixel(uint64_t x, uint64_t y, uint32_t color){
    //gfx_putRectD(x * 2, y * 1, 2, 1, color);
    gfx_plotPixelD(x, y, color);
}

extern bool initCurs;



#include <drivers/mouse.h>

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



void putltr(uint64_t px, uint64_t py, uint32_t color, int letter){
    
    /*

    if (letter == 34 || letter == 43 || letter == 44 || letter == 52){
        py += 4;
    }
    if (letter == 37){
        py += 1;
    }
    */
    for(unsigned int y = 0; y < font_height; y++){

        for(unsigned int x = 0; x < font_width; x++){

            
                
            if (getBit(font_regular[letter][y], font_width - (x))){
                gfx_plotPixel(x + px, y + py, color);
            }
        }
    }
}

uint32_t ASCIItoPK(char character){
    uint32_t searchidx = 0;

    for(searchidx = 0; searchidx < 255; searchidx++){
                if (fontMap[searchidx] == character){
                    break;
                }
            }
    return searchidx;
}

void putstr(const char* str, uint64_t px, uint64_t py, uint32_t color){

    uint64_t x = px;
    uint64_t y = py;
    uint64_t idx = 0;
    uint32_t searchidx = 0;

    for(idx = 0; idx < getstrsz(str); idx++){
            switch (str[idx]){

            case '\n':
                py += font_height + 5;
                x = px;
                y = py;

            break;

            default:

            searchidx = ASCIItoPK(str[idx]);
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

