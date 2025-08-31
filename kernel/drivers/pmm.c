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

// WELCOME YOU PIECE OF SHIT TO MY ABSOLUTELY GARBAGE MEMORY MANAGER!!!
// YOU WILL FUCKING HATE IT HERE, PREPARE TO SHIT YOURSELF IN CRINGE

#include <drivers/ldata.h>
#include <stdint.h>
#include <stdbool.h>
#include <drivers/graphics.h>
#include <utils/utils.h>
#include <drivers/lv1io.h>

#define FLAG_USED 1 << 0

struct freelist_entry{
    // waiting for someone to crash out over 4 bits wasted
    // its not worth it man you're gonna be fine

    // also it can still handle 1099 gb of ram
    // thats still way more than any reasonable human being has

    // "oh a freelist shouldnt have flags!!" SHUT THE FUCK UP!!!

    uint32_t start : 28;
    uint32_t flags : 4;
    uint32_t end;
};

typedef struct freelist_entry freelist_entry_t;

struct freelist_page{
    freelist_entry_t freelist[511];
    void* nextList_addr;
};

typedef struct freelist_page freelist_page_t;

freelist_page_t freelist0 __attribute__((aligned(4096))); // initial free list

void* kmalloc(uint32_t pages){
    return 0;
}



int freep(void* ptr){
       
    

}

extern void crash(const char* str);

uint16_t freelist_findFree(freelist_page_t* list){

    uint32_t lidx = 0;

    for(lidx = 0; lidx < 511; lidx++){
        if (!(list->freelist[lidx].end || list->freelist[lidx].start)){
            break;
        }
    }
    return lidx;
}

uint16_t freelist_split(freelist_page_t* startList, uint16_t entry, uint32_t start, uint32_t end){
    
    freelist_entry_t temp0;


    temp0.start = startList->freelist[entry].start;
    temp0.end = startList->freelist[entry].end;

    if (start < temp0.start){
        return 0;
    }

    if (end > temp0.end){
        return 0;
    }
    
    startList->freelist[entry].start = end;

    uint32_t lidx0 = freelist_findFree(startList);


    startList->freelist[lidx0].start = start;
    startList->freelist[lidx0].end = end;

    uint32_t lidx1 = freelist_findFree(startList);

    startList->freelist[lidx1].start = temp0.start;
    startList->freelist[lidx1].end = start;

    if (startList->freelist[lidx1].end == startList->freelist[lidx1].start){
        startList->freelist[lidx1].start = 0;
        startList->freelist[lidx1].end = 0;
    }

    return lidx0;

}

uint32_t idx = 0;

#define PAGEDIR_PRESENT 1 << 0

void freelist_reserve(freelist_page_t* startList, uint16_t entry){
    startList->freelist[entry].flags |= FLAG_USED;
}


// RECONSTRUCT *WHAT*?! THERE IS _NOTHING_ LEFT
// if you dont get the reference contact any ultrakill player thats alive :trl:
void freelist_reconstruct(freelist_page_t* startList, uint16_t entry){

}

void pmm_Init(){

    // take all memory
    freelist0.freelist[0].start = 0;
    freelist0.freelist[0].end = (mem_Info.mem_total / 4);

    uint32_t item = 0;
    

    //freelist_split(&freelist0 /* startList */, 0 /* entry */ , 0 /* start */, 0 /* end */);

    for(idx = 0; idx < memmap_entries; idx++){
        uint16_t lidx;
        lidx = freelist_split(&freelist0 /* startList */, 0 /* entry */ , CEIL_DIV(memmap[idx].addr, 4096) /* start */, (CEIL_DIV(memmap[idx].addr, 4096) + CEIL_DIV(memmap[idx].len, 4096)) /* end */);
        if (memmap[idx].type != 1){
            freelist_reserve(&freelist0, lidx);
        }
    }



    

    //freelist_split(&freelist0 /* startList */, 0 /* entry */ , 160 + 10 /* start */, 160 + 50 /* end */);

}


#include <drivers/lv2io.h>
#include <krnlBitmaps.h>


void test(){

    char* france = "0x000000000000000000";
    

    putstr(itoa(memmap_entries, france, 10), 5, 40, 0xFFFFFF);
    putstr(itoa(0, france, 10), 5, 60, 0xFFFFFF);

    uint16_t y = 90;
    uint16_t idx = 0;
    for (idx = 0; idx <= memmap_entries; idx++){
      putstr(itoa(memmap[idx].addr, france, 16), 5, y, 0xFFFFFF);
      putstr(itoa(memmap[idx].len, france, 16), 100, y, 0xFFFFFF);
      putstr(itoa(memmap[idx].type, france, 16), 200, y, 0xFFFFFF);
      y += 40;
    }

    y = 90;


    idx = 0;
    for (idx = 0; idx <= 40; idx++){
      putstr(itoa(freelist0.freelist[idx].start, france, 10), 600, y, 0xFFFFFF);
      putstr(itoa(freelist0.freelist[idx].end, france, 10), 700, y, 0xFFFFFF);
      putstr(itoa(freelist0.freelist[idx].flags, france, 2), 800, y, 0xFFFFFF);
      y += 40;
    }


}