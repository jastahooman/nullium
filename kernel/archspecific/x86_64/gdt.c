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

#include <stdbool.h>
#include <stdint.h>

extern void crash(const char* str);

struct GDTpointer {
    unsigned short limit; // gdt size
    unsigned long  base;  // virtual address of GDT
} __attribute__((packed));


// RGDT entry
struct GDTentry{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

typedef struct GDTentry gdt_entry_t;

gdt_entry_t gdt_entries[5];
struct GDTpointer gdt_ptr;

void setGDTgate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

extern void gdt_flush(uint64_t gdt_ptr_addr);

void init_GDT(){
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 3) - 1;
    gdt_ptr.base  = (uint64_t)&gdt_entries;

    setGDTgate(0, 0, 0, 0, 0); //null

    setGDTgate(1, 0, 0xFFFFFFFF, 0x9A, 0xA0); // kernel code

    setGDTgate(2, 0, 0xFFFFFFFF, 0x92, 0xC0);// kernel data


    setGDTgate(3, 0, 0xFFFFFFFF, 0xFA, 0xA0); // user code

    setGDTgate(4, 0, 0xFFFFFFFF, 0xF2, 0xC0); // user data

    gdt_flush((uint64_t)&gdt_ptr);
}