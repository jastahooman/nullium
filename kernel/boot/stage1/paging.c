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

#define PAGETABLE_SHIFT      12
#define PAGEDIR_TABLESHIFT   22

#define KERNEL_PHYSICAL_START 0x00100000
#define CR0_PAGEBIT           1 << 31


void load_pd(void* pd_addr) {
    __asm__ volatile ("mov %0, %%cr3" :: "r" ((uint32_t)pd_addr) : "memory");

}

void enable_paging(void) {
    uint32_t reg;
    __asm__ volatile (
        "mov %%cr0, %0" : "=r"(reg)
    );
    reg |= CR0_PAGEBIT;

    __asm__ volatile (
        "mov %0, %%cr0" 
        :: "r"(reg)
    );
}

extern void crash(const char* str);

#include <drivers/graphics.h>
#include <utils/utils.h>
extern void test();
 
#include <drivers/ldata.h>


//uint32_t pagedir[1024]__attribute__((aligned(4096)));

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {


}

void init_Paging(){
    
    char* teste = "0xFFFFFF";


    test();

    for(;;);
    
    //enable_paging();

    
    


}