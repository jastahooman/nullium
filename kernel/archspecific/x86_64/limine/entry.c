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

// Limine Entry Point


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "limine.h"
#include "stage2.h"

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;



// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}
struct limine_framebuffer *framebuffer;

uint64_t temp;

uint32_t gfx_getPixel(uint64_t x, uint64_t y){
    if (x > framebuffer->width|| y > framebuffer->height){
        return 0x000000;
    }
    volatile uint32_t *fb_ptr = framebuffer->address;
    return fb_ptr[y * (framebuffer->pitch / 4) + x];
}

void gfx_plotPixel(uint64_t x, uint64_t y, uint32_t color){
    if (!(x > framebuffer->width|| y > framebuffer->height)){
        volatile uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[y * (framebuffer->pitch / 4) + x] = color;
    }
    
}

const char* bootLdrName = "Limine";
const char* CPUArch = "x86_64";

void kmain(void) {

    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }
    framebuffer = framebuffer_request.response->framebuffers[0];

    if (framebuffer_request.response == NULL
        || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    gfx_bpp = framebuffer->bpp;
    gfx_resX = framebuffer->width;
    gfx_resY = framebuffer->height;

    stage2_boot();
    
    __asm__ volatile ("cli");
    
    hcf();
}

