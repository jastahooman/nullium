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
#include <stddef.h>
#include <stdbool.h>
#include <utils/utils.h>
#include <utils/utils-x86.h>
#include "idt.h"





struct IDT_Entry idt_entries[256];
struct IDT_Pointer idt_ptr;

extern void IDT_set(uint32_t);
void IDT_SetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags){

    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags | 0x60;

}

#define PIC1_CMD 0x20
#define PIC2_CMD 0xA0

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define PIC_INIT 0x11
#define PIC1_BASE 0x20
#define PIC2_BASE 0x28

#define PIC2_IRQ 0x04
#define PIC1_IRQ 0x02

#define PIC_8086 0x01
#define PIC_UNMASK 0

void init_PIC(){
    outb(PIC1_CMD, PIC_INIT);
    outb(PIC2_CMD, PIC_INIT); 

    outb(PIC1_DATA, PIC1_BASE);
    outb(PIC2_DATA, PIC2_BASE);

    outb(PIC1_DATA, PIC2_IRQ);
    outb(PIC2_DATA, PIC1_IRQ);

    outb(PIC1_DATA, PIC_8086); 
    outb(PIC2_DATA, PIC_8086);

    outb(PIC1_DATA, PIC_UNMASK);
    outb(PIC2_DATA, PIC_UNMASK);
}

void init_IDT(){
    idt_ptr.limit = sizeof(struct IDT_Entry) * 256 - 1;
    idt_ptr.base = (uint32_t) &idt_entries;

    memset(&idt_entries, 0, sizeof(struct IDT_Entry) * 256);

    init_PIC();

    IDT_SetGate(0, (uint32_t)isr0,0x08, 0x8E);
    IDT_SetGate(1, (uint32_t)isr1,0x08, 0x8E);
    IDT_SetGate(2, (uint32_t)isr2,0x08, 0x8E);
    IDT_SetGate(3, (uint32_t)isr3,0x08, 0x8E);
    IDT_SetGate(4, (uint32_t)isr4, 0x08, 0x8E);
    IDT_SetGate(5, (uint32_t)isr5, 0x08, 0x8E);
    IDT_SetGate(6, (uint32_t)isr6, 0x08, 0x8E);
    IDT_SetGate(7, (uint32_t)isr7, 0x08, 0x8E);
    IDT_SetGate(8, (uint32_t)isr8, 0x08, 0x8E);
    IDT_SetGate(9, (uint32_t)isr9, 0x08, 0x8E);
    IDT_SetGate(10, (uint32_t)isr10, 0x08, 0x8E);
    IDT_SetGate(11, (uint32_t)isr11, 0x08, 0x8E);
    IDT_SetGate(12, (uint32_t)isr12, 0x08, 0x8E);
    IDT_SetGate(13, (uint32_t)isr13, 0x08, 0x8E);
    IDT_SetGate(14, (uint32_t)isr14, 0x08, 0x8E);
    IDT_SetGate(15, (uint32_t)isr15, 0x08, 0x8E);
    IDT_SetGate(16, (uint32_t)isr16, 0x08, 0x8E);
    IDT_SetGate(17, (uint32_t)isr17, 0x08, 0x8E);
    IDT_SetGate(18, (uint32_t)isr18, 0x08, 0x8E);
    IDT_SetGate(19, (uint32_t)isr19, 0x08, 0x8E);
    IDT_SetGate(20, (uint32_t)isr20, 0x08, 0x8E);
    IDT_SetGate(21, (uint32_t)isr21, 0x08, 0x8E);
    IDT_SetGate(22, (uint32_t)isr22, 0x08, 0x8E);
    IDT_SetGate(23, (uint32_t)isr23, 0x08, 0x8E);
    IDT_SetGate(24, (uint32_t)isr24, 0x08, 0x8E);
    IDT_SetGate(25, (uint32_t)isr25, 0x08, 0x8E);
    IDT_SetGate(26, (uint32_t)isr26, 0x08, 0x8E);
    IDT_SetGate(27, (uint32_t)isr27, 0x08, 0x8E);
    IDT_SetGate(28, (uint32_t)isr28, 0x08, 0x8E);
    IDT_SetGate(29, (uint32_t)isr29, 0x08, 0x8E);
    IDT_SetGate(30, (uint32_t)isr30, 0x08, 0x8E);
    IDT_SetGate(31, (uint32_t)isr31, 0x08, 0x8E);

    IDT_SetGate(69, (uint32_t)softwareISR, 0x08, 0x8E);
    IDT_SetGate(80, (uint32_t)guiISR, 0x08, 0x8E);

    IDT_SetGate(32, (uint32_t)irq0, 0x08, 0x8E);
    IDT_SetGate(33, (uint32_t)irq1, 0x08, 0x8E);
    IDT_SetGate(34, (uint32_t)irq2, 0x08, 0x8E);
    IDT_SetGate(35, (uint32_t)irq3, 0x08, 0x8E);
    IDT_SetGate(36, (uint32_t)irq4, 0x08, 0x8E);
    IDT_SetGate(37, (uint32_t)irq5, 0x08, 0x8E);
    IDT_SetGate(38, (uint32_t)irq6, 0x08, 0x8E);
    IDT_SetGate(39, (uint32_t)irq7, 0x08, 0x8E);
    IDT_SetGate(40, (uint32_t)irq8, 0x08, 0x8E);
    IDT_SetGate(41, (uint32_t)irq9, 0x08, 0x8E);
    IDT_SetGate(42, (uint32_t)irq10, 0x08, 0x8E);
    IDT_SetGate(43, (uint32_t)irq11, 0x08, 0x8E);
    IDT_SetGate(44, (uint32_t)irq12, 0x08, 0x8E);
    IDT_SetGate(45, (uint32_t)irq13, 0x08, 0x8E);
    IDT_SetGate(46, (uint32_t)irq14, 0x08, 0x8E);
    IDT_SetGate(47, (uint32_t)irq15, 0x08, 0x8E);

    IDT_SetGate(128, (uint32_t)isr128, 0x08, 0x8E); //System calls
    IDT_SetGate(177, (uint32_t)isr177, 0x08, 0x8E); //System calls

    IDT_set((uint32_t)&idt_ptr);

}

char* ISR_ExceptionMessages[] = {
    "Division by 0",
    "Debug",
    "Non Maskable Interrupt",
    "Break Point",
    "Overflow Noticed",
    "Out of Bounds",
    "Invalid Opcode",
    "No Math Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack Fault",
    "General protection fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check", 
    "Reserved 1",
    "Reserved 2",
    "Reserved 3",
    "Reserved 4",
    "Reserved 5",
    "Reserved 6",
    "Reserved 7",
    "Reserved 8",
    "Reserved 9",
    "Reserved 10",
    "Reserved 11",
    "Reserved 12",
    "Reserved 13"
};

extern void crash(const char* str);

extern void syscall_handler(struct InterruptRegisters* regs);

void ISR_Handler(struct InterruptRegisters* regs){
    if (regs->int_no == 69){
        syscall_handler(regs);
        
    }
    if (regs->int_no < 32){
        crash(ISR_ExceptionMessages[regs->int_no]);
    }
}

void *irq_routines[16] = {
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void IRQ_setHandler (int irq, void (*handler)(struct InterruptRegisters *r)){
    irq_routines[irq] = handler;
}

void IRQ_rmHandler(int irq){
    irq_routines[irq] = 0;
}



void IRQ_Handler(struct InterruptRegisters* regs){
    void (*handler)(struct InterruptRegisters *regs);

    handler = irq_routines[regs->int_no - 32];
    

    if (handler){
        handler(regs);
    }

    

    if (regs->int_no >= 8 + 32){
        outb(0xA0, 0x20);
    }

    outb(0x20,0x20);

}
