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

struct GDT_entry{
    uint16_t limit;
    uint16_t base1;
    uint8_t  base2;
    uint8_t  access;
    uint8_t  flags_and_limit;
    uint8_t  base3;
}__attribute__((packed));

struct GDT_unencoded{
    uint32_t limit;
    uint32_t base;
    uint8_t access;
    uint8_t flags;
};

struct GDT_entry GDTentries[6];

struct GDT_unencoded GDTentry;

void GDT_encodeEntry(int item, struct GDT_unencoded source){
    GDTentries[item].base1 = (source.base & 0xFFFF);
    GDTentries[item].base2 = (source.base >> 16) & 0xFF;
    GDTentries[item].base3 = (source.base >> 24) & 0xFF;

    GDTentries[item].limit = (source.limit & 0xFFFF);
    GDTentries[item].flags_and_limit = (source.limit >> 16) & 0x0F;
    GDTentries[item].flags_and_limit |= (source.flags & 0xF0);

    GDTentries[item].access = source.access;
}

extern void setGdt(uint64_t);

void init_GDT(){
    // blank entry
    GDTentry.limit           = 0;
    GDTentry.base            = 0;
    GDTentry.access          = 0;
    GDTentry.flags           = 0;

    GDT_encodeEntry(0, GDTentry);

    GDTentry.limit           = 0xFFFFF;
    GDTentry.base            = 0;
    GDTentry.access          = 0x9A;
    GDTentry.flags           = 0xA;

    GDT_encodeEntry(1, GDTentry);

    GDTentry.limit           = 0xFFFFF;
    GDTentry.base            = 0;
    GDTentry.access          = 0x92;
    GDTentry.flags           = 0xC;

    GDT_encodeEntry(2, GDTentry);


    GDTentry.limit           = 0xFFFFF;
    GDTentry.base            = 0;
    GDTentry.access          = 0xFA;
    GDTentry.flags           = 0xA;

    GDT_encodeEntry(3, GDTentry);


    GDTentry.limit           = 0xFFFFF;
    GDTentry.base            = 0;
    GDTentry.access          = 0xF2;
    GDTentry.flags           = 0xC;

    GDT_encodeEntry(4, GDTentry);

    setGdt((uint64_t)&GDTentries);

}