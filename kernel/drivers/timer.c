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

// Timer Utilities

#include <drivers/timer.h>
#include <stdint.h>
#include <utils/utils-x86.h>
#include <drivers/graphics.h>
uint64_t curr_ticks;


void Timer_Sleep(unsigned int time){
    
    curr_ticks = ticks;
    
    while(ticks - curr_ticks < time){
         shadowTxt(" ", 0 ,0, 0, 0);
    }
}