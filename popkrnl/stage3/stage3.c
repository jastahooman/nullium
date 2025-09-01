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


#include <drivers/lv1io.h>
#include <drivers/lv2io.h>
#include <drivers/graphics.h>



#include <drivers/timer.h>
#include <stdio.h>
#include <drivers/mouse.h>
#include <krnlBitmaps.h>
#include <drivers/keyboard.h>
#include <utils/utils.h>


#include <drivers/ldata.h>


uint8_t cursor = 0;


extern void prevPosCurs();

uint8_t sensitivity = 0;

extern void crash(const char* str);
bool loaded3 = false;


void s3_tick(void){
    
}



extern void crash(const char* str);

int stage3_boot(){
    gfx_putRect(0, 0, gfx_resX, gfx_resY, 0x222222);

    loaded3 = true;

    
    
}