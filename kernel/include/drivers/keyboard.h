/*
    The Nullium Operating System
    Copyright (C) 225, jastahooman

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


extern const uint32_t KB_UNKNOWN;
extern const uint32_t KB_ESC;
extern const uint32_t KB_CTRL;
extern const uint32_t KB_LSHFT;
extern const uint32_t KB_RSHFT;
extern const uint32_t KB_ALT;
extern const uint32_t KB_F1;
extern const uint32_t KB_F2;
extern const uint32_t KB_F3;
extern const uint32_t KB_F4;
extern const uint32_t KB_F5;
extern const uint32_t KB_F6;
extern const uint32_t KB_F7;
extern const uint32_t KB_F8;
extern const uint32_t KB_F9;
extern const uint32_t KB_F1;
extern const uint32_t KB_F11;
extern const uint32_t KB_F12;
extern const uint32_t KB_SCRLCK;
extern const uint32_t KB_HOME;
extern const uint32_t KB_UP;
extern const uint32_t KB_LEFT;
extern const uint32_t KB_RIGHT;
extern const uint32_t KB_DOWN;
extern const uint32_t KB_PGUP;
extern const uint32_t KB_PGDOWN;
extern const uint32_t KB_END;
extern const uint32_t KB_INS;
extern const uint32_t KB_DEL;
extern const uint32_t KB_CAPS;
extern const uint32_t KB_NONE;
extern const uint32_t KB_ALTGR;
extern const uint32_t KB_NUMLCK;


extern const uint32_t kb_keys[128];
extern bool kb_keyPresses[128];

bool kb_detectPress(uint32_t key);