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

#include <drivers/ldata.h>
#include <stdint.h>
#include <stdbool.h>



bool mem_locked = true;


struct memlist{
    bool validElement;
    bool free;
    uint32_t owner;
    
    uint64_t start;
    uint64_t size;
};

struct merge_free{
    uint64_t start;
    uint64_t originalCell;
};

typedef struct memlist memlist_t;

memlist_t allocMap[1048447];
uint32_t allPages = 1048447;

#define FREE 0
#define USED 1

uint32_t startframe;
uint32_t endframe;




void reservePage(uint32_t item, 
                   bool valid, bool free, uint32_t owner, 
                   uint64_t start, uint64_t size){

    allocMap[item].validElement    = valid;
    allocMap[item].free            = free;
    allocMap[item].owner           = owner;

    allocMap[item].size            = size;
    allocMap[item].start           = start;
}

uint32_t maxAllocd = 5;
uint32_t freePgs = 1;
typedef uint64_t page_t;

void statFree(){
    uint64_t idx = 0;
    freePgs = 0;
    for(idx = 0; idx <= maxAllocd; idx++){
        if (allocMap[idx].free && allocMap[idx].validElement){


                freePgs += 1;
        }
    }
}

page_t palloc(uint32_t pages){
    bool foundFree = false;
    uint32_t idx = 0;
    for (idx = 0; idx < maxAllocd; idx++){
        if (allocMap[idx].free){
            if (pages <= allocMap[idx].size){
                foundFree = true;
                break;
            }
        }
    }

    if (!foundFree){
        return 0;
    }
    uint32_t midx = idx;
    allocMap[midx].size -= pages;

    
    
    bool reserved = false;
    for(idx = 0; idx <= maxAllocd; idx++){
        if(!allocMap[idx].validElement){
            if (idx == maxAllocd){
                maxAllocd++;
            }
            reserved = true;
            break;
        }
    }
    if (!reserved){
        return 0;
    }

    reservePage(idx,                           // item     
                true,                          // valid
                false,                         // is free
                0,                             // owner
                allocMap[midx].size,           // start
                pages                          // size
                );

    statFree();

    return (page_t)allocMap[idx].start;
}


void mergep(){
    uint32_t idx0;
    uint32_t idx1;
    uint32_t idx2;


    for (idx2 = 0; idx2 < freePgs; idx2++){
        for(idx0 = 0; idx0 < maxAllocd; idx0++){
            if (allocMap[idx0].free){
                    
                    idx1 = idx0;
                    
                    for(; idx1 < maxAllocd; idx1++){

                        if (allocMap[idx1].free){
                            
                            if (allocMap[idx0].size == allocMap[idx1].start){
                                allocMap[idx1].validElement = 0;
                                allocMap[idx0].size += allocMap[idx1].size;
                                reservePage(idx1,                           // item     
                                            false,                          // valid
                                            false,                          // is free
                                            0,                              // owner
                                            0,                              // start
                                            0                               // size
                                );
                            }
                            
                        }
                    
                    }

            }
        }
    }
}

int freep(page_t page){
    if (page > allPages){
        return -1;
    }

    uint32_t idx = 0;

    bool found = false;
    for(idx = 0; idx <= maxAllocd; idx++){
        found = false;
        if(allocMap[idx].start == page){
            found = true;
            break;
        }
    }
    if (!found){
        return -2;
    }

    allocMap[idx].free = true;

    freePgs += 2;

    mergep();
    statFree();
    
    return 0;
}
#include <stdio.h>
#include <drivers/graphics.h>
#include <utils/utils.h>
#include <drivers/lv1io.h>
void init_mem(){


    char* france = "0xFFFFFFFF";


    reservePage(0,                                                           // item     
                true,                                                        // valid
                false,                                                       // is free
                0,                                                           // owner
                0,                                                           // start
                CEIL_DIV(CEIL_DIV((uint32_t)&ld_start, 1000),4)              // size
                );

    reservePage(1,                                                                            // item     
                true,                                                                         // valid
                true,                                                                         // is free
                0,                                                                            // owner
                CEIL_DIV(CEIL_DIV((uint32_t)&ld_start, 1000),4),                              // start
                (mem_Info.mem_total / 4) - CEIL_DIV(CEIL_DIV((uint32_t)&ld_start, 1000),4)    // size
                );


    uint32_t y = 40;

    palloc(50);

    /*
    // for debugging
    putstr("Valid?", 0, y, 0xFFFF00);
    putstr("Free?", 60, y, 0xFFFF00);
    putstr("Owner", 120, y, 0xFFFF00);
    putstr("Size", 200, y, 0xFFFF00);
    putstr("Start", 300, y, 0xFFFF00);
    putstr("Start (base16)", 450, y, 0xFFFF00);
    putstr("Size (base16)", 600, y, 0xFFFF00);

    
    
    for (uint32_t idx = 0; idx < 10; idx++){
        y += 20;
        putstr(itoa(allocMap[idx].validElement, france, 10), 0, y, 0xFFFF00);
        putstr(itoa(allocMap[idx].free, france, 10), 60, y, 0xFFFF00);
        putstr(itoa(allocMap[idx].owner, france, 10), 120, y, 0xFFFF00);
        
        putstr(itoa(allocMap[idx].size, france, 10), 200, y, 0xFFFF00);
        putstr(itoa(allocMap[idx].size, france, 16), 600, y, 0xFFFF00);

        putstr(itoa(allocMap[idx].start, france, 10), 300, y, 0xFFFF00);
        putstr(itoa(allocMap[idx].start, france, 16), 450, y, 0xFFFF00);
        
    }
    y += 20;
    */


}
