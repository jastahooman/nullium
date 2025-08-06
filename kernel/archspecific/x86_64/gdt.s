; The Nullium Operating System
;    Copyright (C) 2025, jastahooman
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

; NASM assembly


global gdt_flush

gdt_flush:
    ; RDI holds the 64-bit address of the GDT pointer structure
    lgdt [rdi]

    ; Reload CS register with a far jump to a label
    ; This forces the CPU to use the new code segment (selector 0x08)
    mov ax, 0x10      ; Selector for the new data segment (0x10)
    mov ds, ax        ; Reload Data Segment
    mov es, ax        ; Reload Extra Segment
    mov ss, ax        ; Reload Stack Segment
    ; fs and gs can be set to ax or 0 as needed

    ; Push the new code segment selector (0x08) and the address of the next instruction
    push 0x08
    lea rax, [rel .flush_cs]
    push rax

    ; Perform a far return to flush CS
    retfq

.flush_cs:
    ret