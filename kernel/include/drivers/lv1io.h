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


void gfx_plotPixelD(uint64_t x, uint64_t y, uint32_t color);
uint32_t gfx_getPixelD(uint64_t x, uint64_t y);

struct nm_meminfo{
    uint64_t mem_lower;
    uint64_t mem_upper;
    uint64_t mem_total;
};
extern struct nm_meminfo mem_Info;

#define CPUARCH_i386 0
#define CPUARCH_x86_64 1



extern uint32_t total_tags;
extern uint16_t memmap_entries;

extern uint8_t cpuarch;

struct memmap_entry_x86
{
  uint64_t addr;
  uint64_t len;
  #define MULTIBOOT_MEMORY_AVAILABLE              1
  #define MULTIBOOT_MEMORY_RESERVED               2
  #define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
  #define MULTIBOOT_MEMORY_NVS                    4
  #define MULTIBOOT_MEMORY_BADRAM                 5
  uint32_t type;
  uint32_t zero;
};

typedef struct memmap_entry_x86 memmap_entry_t;

extern memmap_entry_t memmap[20];