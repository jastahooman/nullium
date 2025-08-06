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

void putltr(uint64_t px, uint64_t py, uint32_t color, int letter);

void putRect(uint64_t px, uint64_t py, uint32_t color);
void gfx_wireRect(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color);

void putstr(const char* str, uint64_t px, uint64_t py, uint32_t color);

void gfx_putRect(uint64_t px, uint64_t py, uint64_t width, uint64_t height, uint32_t color);
void drawCurs(uint64_t posx, uint64_t posy, uint8_t cursor);

void shadowTxt(const char* str, uint64_t px, uint64_t py, uint32_t color, uint32_t shadowClr);