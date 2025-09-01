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

// WELCOME YOU PIECE OF SHIT TO MY ABSOLUTELY GARBAGE MEMORY MANAGER!!!
// YOU WILL FUCKING HATE IT HERE, PREPARE TO SHIT YOURSELF IN CRINGE

#include <drivers/ldata.h>
#include <stdint.h>
#include <stdbool.h>
#include <drivers/graphics.h>
#include <utils/utils.h>
#include <drivers/lv1io.h>

#define FLAG_USED 1 << 0
#define FLAG_FREE 1 << 1
#define FLAG_PAGED 1 << 2

struct freelist_entry{
    // waiting for someone to crash out over 4 bits wasted
    // its not worth it man you're gonna be fine

    // also it can still handle 1099 gb of ram (i'm not even in long mode)
    // thats still way more than any reasonable human being has

    // "oh a freelist shouldnt have flags!!" plz stfu </3

    uint32_t start : 28;
    uint32_t flags : 4;
    uint32_t end;
};

void freelist_housekeep();

typedef struct freelist_entry freelist_entry_t;
//#define LIST_MAX 511
//#define LIST_DANGER 500

#define LIST_MAX 8192

struct freelist_page{
    freelist_entry_t freelist[LIST_MAX];
};

typedef struct freelist_page freelist_t;

freelist_t freelist __attribute__((aligned(4096))); // initial free list

extern void crash(const char* str);

#include <drivers/lv2io.h>
#include <krnlBitmaps.h>
#include <stdio.h>
#include <stage3/nterm.h>

uint16_t freelist_findBlank(freelist_t* list){

    uint32_t lidx = 0;

    for(lidx = 0; lidx < LIST_MAX; lidx++){
        if (!(list->freelist[lidx].flags)){
            break;
        }
    }
    if (lidx == LIST_MAX){
        crash("List Overflow");
        return 9000;
    }
    return lidx;
}

uint16_t freelist_split(uint16_t entry, uint32_t start, uint32_t end){
    

    freelist_t* startList = &freelist; // jank

    freelist_entry_t temp0;


    temp0.start = startList->freelist[entry].start;
    temp0.end = startList->freelist[entry].end;
    temp0.flags = startList->freelist[entry].flags;

    if (start < temp0.start){
        return 0;
    }

    if (end > temp0.end){
        return 0;
    }
    
    startList->freelist[entry].start = end;


    if (startList->freelist[entry].end == startList->freelist[entry].start){
        startList->freelist[entry].flags = 0;
        startList->freelist[entry].start = 0;
        startList->freelist[entry].end = 0;
    }


    uint32_t lidx0 = freelist_findBlank(startList);



    startList->freelist[lidx0].start = start;
    startList->freelist[lidx0].end = end;
    startList->freelist[lidx0].flags |= FLAG_USED;

    if (startList->freelist[lidx0].end == startList->freelist[lidx0].start){
        startList->freelist[lidx0].flags = 0;
        startList->freelist[lidx0].start = 0;
        startList->freelist[lidx0].end = 0;
        
    }

    uint32_t lidx1 = freelist_findBlank(startList);


    startList->freelist[lidx1].start = temp0.start;
    startList->freelist[lidx1].end = start;
    startList->freelist[lidx1].flags |= FLAG_USED;

    if (startList->freelist[lidx1].end == startList->freelist[lidx1].start){
        startList->freelist[lidx1].flags = 0;
        startList->freelist[lidx1].start = 0;
        startList->freelist[lidx1].end = 0;
    }
    

    
    return lidx0;

}

uint32_t idx = 0;



void* palloc(uint32_t pages){
    freelist_t* baseList = &freelist; // jank

    uint16_t idx = 0;

    while (true){
        
        if (getBit(baseList->freelist[idx].flags, 1) && getBit(baseList->freelist[idx].flags, 2)){
            if (!(baseList->freelist[idx].end - baseList->freelist[idx].start < pages)){
                break;
            }
        }
        
        
        idx += 1;



    }

    uint32_t flIdx = freelist_split(idx, baseList->freelist[idx].start, baseList->freelist[idx].start + pages);

    terminal_print("[DBG] ", 0, 12);
    terminal_print("PMM: Reserved ", 0, 15);
    terminal_print(itoa(pages, "0xFFFFFF", 10), 0, 15);
    terminal_print(" page(s) (starts at page 0x", 0, 15);
    terminal_print(itoa((baseList->freelist[flIdx].start), "0xFFFFFFFFFFF", 16), 0, 15);
    terminal_print(")\n", 0, 15);

    freelist_housekeep();

    

    return (void*)(baseList->freelist[flIdx].start * 4096);
}

uint16_t freelist_findItem(uint32_t page){

    uint32_t lidx = 0;

    for(lidx = 0; lidx < LIST_MAX; lidx++){
        if (freelist.freelist[lidx].flags && (freelist.freelist[lidx].start == page)){
            break;
        }
    }
    return lidx;
}


void freelist_housekeep(){



    uint32_t blank = 0;
    uint32_t idx = 0;

    uint32_t lidx = 0;

    uint32_t pageid0;
    uint32_t pageid1;
    uint32_t loop = freelist_findBlank(&freelist);
    
    for(idx = 1; idx < loop; idx++){
        for(lidx = 1; lidx < loop; lidx++){
            pageid1 = freelist_findItem(freelist.freelist[lidx].end);
            pageid0 = freelist_findItem(freelist.freelist[lidx].start);

            if ((getBit(freelist.freelist[pageid0].flags, 1) && getBit(freelist.freelist[pageid0].flags, 2)) &&
                (getBit(freelist.freelist[pageid1].flags, 1) && getBit(freelist.freelist[pageid1].flags, 2))
            ){
                
                freelist.freelist[pageid1].start = freelist.freelist[pageid0].start;


                freelist.freelist[pageid0].flags = 0;
                freelist.freelist[pageid0].end = 0;
                freelist.freelist[pageid0].start =0;
            }
        }
    }

    

    
}
    


int freep(void* ptr){
    uint32_t pageid = ((uint32_t)ptr / 4096);
    freelist_t* startList = &freelist;

    terminal_print("[DBG] ", 0, 12);
    terminal_print("PMM: Request to free page ", 0, 15);
    terminal_print(itoa(pageid, "0xFFFFFFFFFFF", 16), 0, 15);
    terminal_print(".\n", 0, 15);

    terminal_print("[DBG] ", 0, 12);
    terminal_print("Checking if page is valid to free..\n", 0, 15);

    bool fail = true;
    uint32_t idx = 0;

    for (idx = 0; idx < LIST_MAX; idx++){
        if (startList->freelist[idx].start == pageid){
            fail = false;
            terminal_print("[DBG] ", 0, 12);
            terminal_print("Needed page found in list.\n", 0, 15);
            break;
        }
    }

    if (fail){
        terminal_print("[DBG] ", 0, 12);
        terminal_print("Page search failed.\n", 0, 15);
        return 1;
    }
    
    startList->freelist[idx].flags |= FLAG_FREE;
    terminal_print("[DBG] ", 0, 12);
    terminal_print("Page Freed OK.\n", 0, 15);

    freelist_housekeep();


    return 0;
}



void pmm_Init(){

    // take all memory
    freelist.freelist[0].start = 0;
    freelist.freelist[0].end = (mem_Info.mem_total / 4);
    freelist.freelist[0].flags |= FLAG_USED;

    uint32_t item = 0;
    
    
    
    for(idx = 0; idx < memmap_entries; idx++){
        uint16_t lidx;
        lidx = freelist_split(0 /* entry */ , memmap[idx].addr / 4096 /* start */, (memmap[idx].addr / 4096 + CEIL_DIV(memmap[idx].len, 4096)) /* end */);
        
        if (memmap[idx].type == 1){
            freelist.freelist[lidx].flags |= FLAG_FREE;
        }
    }

    for (idx = 0; idx < LIST_MAX; idx++){
        if (freelist.freelist[idx].start == 256){
            freelist_split(idx, ((uint32_t)&ld_start/4096), CEIL_DIV((uint32_t)&ld_end, 4096));
            break;
        }
    }
    


}