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



uint32_t gfx_getPixel(uint64_t x, uint64_t y);
void gfx_plotPixel(uint64_t x, uint64_t y, uint32_t color);

void gfx_copyBuffer();

void putltr(uint64_t px, uint64_t py, uint32_t color, int letter);

void putltr_txtmode(uint64_t px, uint64_t py, uint32_t color, char letter);

void putRect(uint64_t px, uint64_t py, uint32_t color);
void gfx_wireRect(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color);

void putstr(const char* str, uint64_t px, uint64_t py, uint32_t color);

void gfx_putRect(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color);
void drawCurs(uint8_t cursor);

void shadowTxt(const char* str, uint64_t px, uint64_t py, uint32_t color, uint32_t shadowClr);


void gfx_ldMouseBuffer(uint32_t px, uint32_t py);
void gfx_mkMouseBuffer(uint32_t px, uint32_t py);

uint32_t ASCIItoPK(char character);