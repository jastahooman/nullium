/*
    The Nullium Driver Software Development Kit

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


#define DRIVER_SDK_DEVICETYPE_KEYBOARD 0
#define DRIVER_SDK_DEVICETYPE_MOUSE 1
#define DRIVER_SDK_DEVICETYPE_STORMEDIUM 2
#define DRIVER_SDK_DEVICETYPE_CDROM 3
#define DRIVER_SDK_DEVICETYPE_GPU 4
#define DRIVER_SDK_DEVICETYPE_FILESYSTEM 5

#include <stdbool.h>
#include <stdint.h>

void dsdk_setup(uint16_t devicetype);