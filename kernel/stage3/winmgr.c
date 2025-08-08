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
    

// Window manager...
#include <drivers/lv1io.h>
#include <drivers/graphics.h>
#include <drivers/lv2io.h>
#include <krnlBitmaps.h>


void drawMenuBar(){
    gfx_putRect(0, 0, gfx_resX, 19, 0x000000);
    gfx_putRect(0, 0, gfx_resX, 17, 0xFFFFFF);

    unsigned int idx = 0;
    // Nullium logo
    for(unsigned int y = 0; y < 12; y++){

        for(unsigned int x = 0; x < 12; x++){

            
                
            if (menuBtn[idx] == 1){
                gfx_plotPixel(x + 2, y + 2, 0x000000);

            }
            idx++;
        }
    }


}