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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

extern const char* BootProtocol;
extern const char* PCtype;
extern const char* PCfirmware;
extern const char* CPUArch;


void gfx_plotPixel(uint64_t x, uint64_t y, uint32_t color);
uint32_t gfx_getPixel(uint64_t x, uint64_t y);