/*
    PopKernel Bitmaps
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

#include <stdbool.h>
#include <stdint.h>

void renderTerm();

struct terminal_char{
    char ltr;
    uint8_t bgcolor;
    uint8_t fgcolor;
};

extern uint16_t print_row;
extern uint16_t print_col;

typedef struct terminal_char terminal_char_t;

void terminal_init(terminal_char_t* fb, uint16_t width, uint16_t height, uint16_t offsetX, uint16_t offsetY);
void terminal_putChar(char ltr, uint16_t row, uint16_t col, uint8_t colorBG, uint8_t colorFG); 
void terminal_writeStr(const char* str, uint16_t row, uint16_t col, uint8_t colorBG, uint8_t colorFG);
void terminal_print(const char* str, uint8_t colorBG, uint8_t colorFG);
void terminal_scrollUp();