/*
    The Nullium Terminal Emulator
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

uint8_t textmode_fb[24576]; // replace with something not statically allocated

struct terminal_char{
    uint8_t ltr;
    uint8_t bgcolor;
    uint8_t fgcolor;
};

typedef struct terminal_char terminal_char_t;

void renderTerm(){
    uint32_t x;
    uint32_t y;
    for (y = 0; y < gfx_resY / (font_height + 1); y++){
        for (x = 0; x < gfx_resX / (font_width + 1); x++){
            gfx_putRect(x * (font_width + 1), y * (font_height + 1), font_width + 1, font_height + 1, 0xFF0000);
            putstr("A", x * (font_width + 1), y * (font_height + 1), 0xFFFFFF);
        }
    }
}