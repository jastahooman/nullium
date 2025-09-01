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
#include <utils/utils.h>
#include <utils/utils-x86.h>
#include "multiboot2.h"
#include <drivers/graphics.h>

const char* PCfirmware;
const char* BootProtocol;
const char* CPUArch;
const char* PCtype;

extern long gfx_resX;
extern long gfx_resY;
extern long gfx_bpp;

struct nm_meminfo{
  uint64_t mem_lower;
  uint64_t mem_upper;
  uint64_t mem_total;
};

struct nm_boot_fb{
    uint16_t type;
    uint16_t flags;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    void * addr;
};


struct nm_boot_fb fb_Info;
struct nm_meminfo mem_Info;
extern void stage2_boot(void);






uint32_t gfx_getPixelD(uint64_t x, uint64_t y){
  if (!(x > fb_Info.width || y > fb_Info.height)){
        volatile uint32_t *fb_ptr = fb_Info.addr;
        uint32_t color = fb_ptr[y * (fb_Info.pitch / 4) + x];
        return color;
  }
  return 0;
}

extern uint32_t cursor_posX;
extern uint32_t cursor_posY;

extern uint32_t cursor_posX_p;
extern uint32_t cursor_posY_p;


uint32_t gfx_cursMkBuffer(uint32_t px, uint32_t py);
uint32_t gfx_restoreBuffer(uint32_t px, uint32_t py);


void gfx_plotPixelD(uint64_t x, uint64_t y, uint32_t color){

    bool bufferOW = false;

    if (!(x > fb_Info.width || y > fb_Info.height)){
        volatile uint32_t *fb_ptr = fb_Info.addr;
        fb_ptr[y * (fb_Info.pitch / 4) + x] = color;
    }
}


struct VBEdriverInfo{
    
};

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

uint8_t cpuarch;
memmap_entry_t memmap[20];

uint16_t memmap_entries = 0;
uint32_t total_tags = 0;

void stage1_boot (unsigned long magic, unsigned long addr){  
    struct multiboot_tag *tag;



    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
      for(;;);
    }

    for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag 
                                       + ((tag->size + 7) & ~7)))
    {
        switch (tag->type){ 
        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
          
          struct multiboot_tag_basic_meminfo *meminfo = (struct multiboot_tag_basic_meminfo *) tag;
          mem_Info.mem_lower = meminfo->mem_lower;
          mem_Info.mem_upper = meminfo->mem_upper;
          mem_Info.mem_total = meminfo->mem_lower + meminfo->mem_upper;
          
          break;



        case MULTIBOOT_TAG_TYPE_VBE:
          struct multiboot_tag_vbe* vbe_modeInfo = (struct multiboot_tag_vbe*) tag;
        break;

        case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
          {
            multiboot_uint32_t color;
            unsigned i;
            struct multiboot_tag_framebuffer *tagfb
              = (struct multiboot_tag_framebuffer *) tag;
            
            fb_Info.height = tagfb->common.framebuffer_height;
            fb_Info.width = tagfb->common.framebuffer_width;
            fb_Info.pitch = tagfb->common.framebuffer_pitch;
            fb_Info.depth = tagfb->common.framebuffer_bpp;
            fb_Info.addr = (void *) (unsigned long) tagfb->common.framebuffer_addr;
            //fb_Info.addr += 0xC0000000;

            switch (tagfb->common.framebuffer_type)
              {
              case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
                
                  unsigned best_distance, distance;
                  struct multiboot_color *palette;
            
                  palette = tagfb->framebuffer_palette;

                  

                  color = 0;
                  best_distance = 4*256*256;
            
                  for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
                    {
                      distance = (0xff - palette[i].blue) 
                        * (0xff - palette[i].blue)
                        + palette[i].red * palette[i].red
                        + palette[i].green * palette[i].green;
                      if (distance < best_distance)
                        {
                          color = i;
                          best_distance = distance;
                        }
                    }
              
              break;
              

              case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) 
                  << tagfb->framebuffer_blue_field_position;
                break;

              default:
                color = 0xffffffff;
                break;
            }

            break;
          }
          case MULTIBOOT_TAG_TYPE_MMAP:

              memmap_entries = 0;
              multiboot_memory_map_t *mmap;


                    
            for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
              (multiboot_uint8_t *) mmap 
                < (multiboot_uint8_t *) tag + tag->size;
              mmap = (multiboot_memory_map_t *) 
                ((unsigned long) mmap
                + ((struct multiboot_tag_mmap *) tag)->entry_size)){

                  memmap[memmap_entries].addr = mmap->addr;
                  memmap[memmap_entries].len = mmap->len;
                  memmap[memmap_entries].type = mmap->type;

                  memmap_entries++;
                      
            }

              

              break;
        }
        
        total_tags++;
        
    }

    gfx_resX = fb_Info.width;
    gfx_resY = fb_Info.height;
    gfx_bpp  = fb_Info.depth;

    // power pc port planned

    BootProtocol = "GNU Multiboot2";
    PCtype = "IBM PC or compatible";
    PCfirmware = "BIOS or UEFI in CSM mode";
    CPUArch = "32-bit x86";

    stage2_boot();
    
}