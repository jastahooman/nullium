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
#include <stdbool.h>
#include "../../utils.h"

struct GDTentry{
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t flags;
    uint8_t base_high;
}__attribute__((packed));

struct GDTpointer{
    uint16_t limit;
    unsigned int base;
}__attribute__((packed));

struct TSSentry{
	uint32_t prev_tss;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t esp1;
	uint32_t ss1;
	uint32_t esp2;
	uint32_t ss2;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t es;
	uint32_t cs;
	uint32_t ss;
	uint32_t ds;
	uint32_t fs;
	uint32_t gs;
	uint32_t ldt;
	uint32_t trap;
	uint32_t iomap_base;
} __attribute__((packed));
extern void GDT_set(uint32_t);
extern void TSS_set();

struct GDTentry GDT[6];
struct GDTpointer GDT_pointer;
struct TSSentry tss_entry;



void GDT_setGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){

    GDT[num].base_low = (base & 0xFFFF);
    GDT[num].base_middle = (base >> 16) & 0xFF;
    GDT[num].base_high = (base >> 24) & 0xFF;

    GDT[num].limit = (limit & 0xFFFF);
    GDT[num].flags = (limit >> 16) & 0x0F;
    GDT[num].flags |= (gran & 0xF0);

    GDT[num].access = access;

}

void TSS_MkEntry(uint32_t num, uint16_t ss0, uint32_t esp0){
    uint32_t base = (uint32_t) &tss_entry;
    uint32_t limit = base + sizeof(tss_entry);

    GDT_setGate(num, base, limit, 0xE9, 0x00);
    memset(&tss_entry, 0, sizeof(tss_entry));

    tss_entry.ss0 = ss0;
    tss_entry.esp0 = esp0;

    tss_entry.cs = 0x08 | 0x3;
    tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x10 | 0x3;
}

void init_GDT(){
    GDT_pointer.limit = (sizeof(struct GDTentry) * 6) - 1;
    GDT_pointer.base = (uint32_t)&GDT;

    GDT_setGate(0,0,0,0,0);                  //Null
    GDT_setGate(1,0,0xFFFFFFFF, 0x9A, 0xCF); //Kernel code
    GDT_setGate(2,0,0xFFFFFFFF, 0x92, 0xCF); //Kernel data
    GDT_setGate(3,0,0xFFFFFFFF, 0xFA, 0xCF); //User code
    GDT_setGate(4,0,0xFFFFFFFF, 0xF2, 0xCF); //User data
    TSS_MkEntry(5,0x10, 0x0);

    GDT_set((uint32_t)&GDT_pointer);
    TSS_set();
}