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

#include <drivers/pmm.h>


#define PAGE_SIZE             4096
#define PAGE_TABLE_SIZE       1024
#define PAGE_DIRECTORY_SIZE   1024

typedef struct {
    uint32_t present    : 1;
    uint32_t rw         : 1;
    uint32_t user       : 1;
    uint32_t write_thru : 1;
    uint32_t cache_dis  : 1;
    uint32_t accessed   : 1;
    uint32_t dirty      : 1;
    uint32_t pat        : 1;
    uint32_t global     : 1;
    uint32_t available  : 3;
    uint32_t frame_addr : 20;
} page_attrs_t;




#define PAGETABLE_PRESENT 1 << 0
#define PAGETABLE_RW      1 << 1
#define PAGETABLE_USER    1 << 2

#define PAGEDIR_PRESENT 1 << 0
#define PAGEDIR_RW      1 << 1
#define PAGEDIR_USER    1 << 2


#define NUM_SHIFT   22

#define KERNEL_PHYSICAL_START 0x00100000
#define CR0_PAGEBIT           1 << 31

extern void crash(const char* str);

#include <drivers/graphics.h>
#include <utils/utils.h>
#include <drivers/lv1io.h>
#include <stage3/nterm.h>
extern void test();
 
#include <drivers/ldata.h>

extern void loadPageDirectory(uint32_t*);
extern void enablePaging();

uint32_t pagedir[1024]__attribute__((aligned(4096)));
uint32_t* pagetable0;

uint8_t mk_table(void* addr, uint32_t flags, uint32_t dir_flags){

    uint32_t* pagetable = palloc(1);


    

    if (!(((uint32_t)addr/4096) == CEIL_DIV((uint32_t)addr, 4096))){
        return 1;
    }
    uint32_t i;
    for(i = 0; i < 1024; i++){
        pagetable[i] = ((uint32_t)addr + (i * 0x1000)) | flags;
    }

    pagedir[(uint32_t)addr >> NUM_SHIFT] = ((uint32_t)pagetable) | dir_flags;
}


void init_Paging(){
    
    char* teste = "0xFFFFFF";


    uint32_t i;
    for(i = 0; i < 1024; i++){
        // not present, system, but is allowed to be written to
        pagedir[i] = PAGEDIR_RW;
    }

    mk_table(0x0, PAGETABLE_PRESENT | PAGETABLE_RW, PAGEDIR_PRESENT | PAGETABLE_RW);
    mk_table((void*)(((uint32_t)fb_Info.addr/4096) * 4096), PAGETABLE_PRESENT | PAGETABLE_RW, PAGEDIR_PRESENT | PAGETABLE_RW);

    
    
    

    

    loadPageDirectory(pagedir);
    enablePaging();
    


}

void init_vmm(){
    //terminal_print("Starting VMM.", 0, 7);
}