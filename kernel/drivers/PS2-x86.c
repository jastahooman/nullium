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

#include <stdbool.h>
#include <stdint.h>
#include "drivers/keyboard.h"
#include <utils/utils-x86.h>
#include <utils/utils.h>
#include <drivers/idt.h>

#include <stdio.h>

#include <drivers/graphics.h>
#include <krnlBitmaps.h>

bool kb_capsOn;
bool kb_capsLock;

const uint32_t KB_UNKNOWN = 0xFFFFFFFF;
const uint32_t KB_ESC = 0xFFFFFFFF - 1;
const uint32_t KB_CTRL = 0xFFFFFFFF - 2;
const uint32_t KB_LSHFT = 0xFFFFFFFF - 3;
const uint32_t KB_RSHFT = 0xFFFFFFFF - 4;
const uint32_t KB_ALT = 0xFFFFFFFF - 5;
const uint32_t KB_F1 = 0xFFFFFFFF - 6;
const uint32_t KB_F2 = 0xFFFFFFFF - 7;
const uint32_t KB_F3 = 0xFFFFFFFF - 8;
const uint32_t KB_F4 = 0xFFFFFFFF - 9;
const uint32_t KB_F5 = 0xFFFFFFFF - 10;
const uint32_t KB_F6 = 0xFFFFFFFF - 11;
const uint32_t KB_F7 = 0xFFFFFFFF - 12;
const uint32_t KB_F8 = 0xFFFFFFFF - 13;
const uint32_t KB_F9 = 0xFFFFFFFF - 14;
const uint32_t KB_F10 = 0xFFFFFFFF - 15;
const uint32_t KB_F11 = 0xFFFFFFFF - 16;
const uint32_t KB_F12 = 0xFFFFFFFF - 17;
const uint32_t KB_SCRLCK = 0xFFFFFFFF - 18;
const uint32_t KB_HOME = 0xFFFFFFFF - 19;
const uint32_t KB_UP = 0xFFFFFFFF - 20;
const uint32_t KB_LEFT = 0xFFFFFFFF - 21;
const uint32_t KB_RIGHT = 0xFFFFFFFF - 22;
const uint32_t KB_DOWN = 0xFFFFFFFF - 23;
const uint32_t KB_PGUP = 0xFFFFFFFF - 24;
const uint32_t KB_PGDOWN = 0xFFFFFFFF - 25;
const uint32_t KB_END = 0xFFFFFFFF - 26;
const uint32_t KB_INS = 0xFFFFFFFF - 27;
const uint32_t KB_DEL = 0xFFFFFFFF - 28;
const uint32_t KB_CAPS = 0xFFFFFFFF - 29;
const uint32_t KB_NONE = 0xFFFFFFFF - 30;
const uint32_t KB_ALTGR = 0xFFFFFFFF - 31;
const uint32_t KB_NUMLCK = 0xFFFFFFFF - 32;


const uint32_t kb_keys[128] = {
KB_UNKNOWN,KB_ESC,'1','2','3','4','5','6','7','8',
'9','0','-','=','\b','\t','q','w','e','r',
't','y','u','i','o','p','[',']','\n',KB_CTRL,
'a','s','d','f','g','h','j','k','l',';',
'\'','`',KB_LSHFT,'\\','z','x','c','v','b','n','m',',',
'.','/',KB_RSHFT,'*',KB_ALT,' ',KB_CAPS,KB_F1,KB_F2,KB_F3,KB_F4,KB_F5,KB_F6,KB_F7,KB_F8,KB_F9,KB_F10,KB_NUMLCK,KB_SCRLCK,KB_HOME,KB_UP,KB_PGUP,'-',KB_LEFT,KB_UNKNOWN,KB_RIGHT,
'+',KB_END,KB_DOWN,KB_PGDOWN,KB_INS,KB_DEL,KB_UNKNOWN,KB_UNKNOWN,KB_UNKNOWN,KB_F11,KB_F12,KB_UNKNOWN,
};

bool kb_keyPresses[128];

extern void crash(const char* str);

bool kb_detectPress(uint32_t key){
    uint32_t idx = 0;
    while (kb_keys[idx] != key){
        idx++;
        if (idx > 128){
            return 0;
        }
    }
    
    return kb_keyPresses[idx];
}

void keyboardHandler(struct InterruptRegisters *regs){
    uint8_t kb_byte = inb(0x60);
    uint8_t scanCode = kb_byte & 0x7F; // key
    bool press = kb_byte & 0x80; // released or pressed?
    
    switch(scanCode){
        case 42:
            if (press == 0){
                kb_capsOn = true;
            }else{
                kb_capsOn = false;
            }
        break;
        case 58:
        if (!kb_capsLock && press == 0){
            kb_capsLock = true;
        }else if (kb_capsLock && press == 0){
            kb_capsLock = false;
        }

        break;
        default:
        if (!press){
            kb_keyPresses[scanCode] = true;
        } else {
            kb_keyPresses[scanCode] = false;
        }
        break;
    }
}

bool AUX_available = false;

uint8_t mouse_cycle=0;     //unsigned char

char mouse_byte[3];    //signed char
char mouse_x=0;         //signed char
char mouse_y=0;         //signed char




inline void mouse_wait(uint8_t a_type) //unsigned char
{
  uint32_t time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(time_out--) //Data
    {
      if((inb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(time_out--) //Signal
    {
      if((inb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}
bool mouse_newIO = false;

void mouse_acknowledge(){
    mouse_newIO = false;
}

bool mouse_btnPressed[3];

void mouse_handler(struct InterruptRegisters *regs){

    switch(mouse_cycle)
    {
    case 0:
        mouse_byte[0]=inb(0x60);
        mouse_cycle++;
        break;
    case 1:
        mouse_byte[1]=inb(0x60);
        mouse_cycle++;
        break;
    case 2:
        mouse_byte[2]=inb(0x60);
        mouse_x=mouse_byte[1];
        mouse_y=mouse_byte[2];
        mouse_cycle=0;
        mouse_newIO = true;

        mouse_btnPressed[0] = getBit(mouse_byte[0], 1);
        mouse_btnPressed[1] = getBit(mouse_byte[0], 0);
        mouse_btnPressed[2] = getBit(mouse_byte[0], 2);
        
        break;
    }
}

inline void mouse_write(uint8_t a_write)
{
  //Wait to be able to send a command
  mouse_wait(1);
  //Tell the mouse we are sending a command
  outb(0x64, 0xD4);
  //Wait for the final part
  mouse_wait(1);
  //Finally write
  outb(0x60, a_write);
}

uint8_t mouse_read()
{
  //Get's response from mouse
  mouse_wait(0); 
  return inb(0x60);
}
#include <drivers/driversdkmnt-x86.h>

void PS2_Init(){

    // Keyboard

    // disable caps, shift
    kb_capsOn = false;
    kb_capsLock = false;

    while ((inb(0x64) & (1 << 0)) != 0){
        inb(0x60);
    }

    IRQ_setHandler(1, &keyboardHandler);
    dsdk_setup(DRIVER_SDK_DEVICETYPE_KEYBOARD);
    // Mouse:

    uint8_t status;  //unsigned char

    //Enable the auxiliary mouse device
    mouse_wait(1);
    outb(0x64, 0xA8);

    //Enable the interrupts
    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status=(inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    //Tell the mouse to use default settings
    mouse_write(0xF6);
    mouse_read();  //Acknowledge

    //Enable the mouse
    mouse_write(0xF4);
    mouse_read();  //Acknowledge

    //Setup the mouse handler
    IRQ_setHandler(12, &mouse_handler);
    dsdk_setup(DRIVER_SDK_DEVICETYPE_MOUSE);

}
