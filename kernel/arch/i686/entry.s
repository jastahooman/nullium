; The Nullium Operating System
; Copyright (C) 2025, jastahooman
;
;    This program is free software: you can redistribute it and/or modify
;    it under the terms of the GNU General Public License as published by
;    the Free Software Foundation, either version 3 of the License, or
;    (at your option) any later version.
;
;    This program is distributed in the hope that it will be useful,
;    but WITHOUT ANY WARRANTY; without even the implied warranty of
;    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;    GNU General Public License for more details.
;
;    You should have received a copy of the GNU General Public License
;    along with this program.  If not, see <https://www.gnu.org/licenses/>.

; NASM 32-bit x86 assembly

MB_MAGIC            equ 0xE85250D6
MB_ARCHITECTURE     equ 0
MB_HEADER_LENGTH    equ (MB_HeaderEnd - MB_Header)
MB_CHECKSUM         equ -(MB_MAGIC + MB_ARCHITECTURE + MB_HEADER_LENGTH)

STACK_SIZE          equ 0x4000

MB_ENTRY_ADDR


section .multiboot
ALIGN 4
MB_Header:
    DD MB_MAGIC
    DD MB_ARCHITECTURE
    DD MB_HEADER_LENGTH
    DD MB_CHECKSUM

extern _start
extern _end_data
extern _end

ADDR_Tag:
    DW 2
    DW 1
    DD (ADDR_TagEnd - ADDR_Tag)

    DD MB_Header
    DD _start
    DD _end_data
    DD _end
ADDR_TagEnd:

ENTRYADDR_Tag:
    DW 3
    DW 1
    DD (ADDR_TagEnd - ADDR_Tag)

    DD OS_Start
ENDADDR_TagEnd:

; behold: the ultimate fuckshit implementation
DW 0
DW 0
DD 0

align 8
FB_Tag:
    
    DW 5
    DW 1
    DD 20

    DD 640
    DD 480
    DD 32
FB_TagEnd:

MB_HeaderEnd:



section .text

OS_Start:
    mov esp, stack_top
    push ebx
    push eax
    extern stage1_boot
    call stage1_boot

OS_Halt:
    hlt
    JMP OS_Halt



section .bss
align 16
stack_bottom:
    RESB 16384 * 8
stack_top:
