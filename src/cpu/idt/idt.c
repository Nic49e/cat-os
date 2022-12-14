#include "idt.h"
#include "lib/stdio.h"
#include "lib/lock.h"
#include "cpu/idt/idt.h"

spinlock_t idt_lock=LOCK_INIT;
static idt_gate_t idt[256];
void* isr[256];
extern void* isr_stubs[];
static idtptr_t idtr;

const char* exception_names[] = {
    "Division By Zero",
    "Debug",
    "NMI",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "???",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "???",
    "x87 Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Exception",
    "Virtualisation",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "???",
    "Security"
};

void idt_load(){
    __asm__ volatile("lidt %0"
                     :
                     : "m"(idtr));
    __asm__ volatile("sti");
}



static void encode_idt_entry(uint8_t vector, void* handler, uint8_t flags) {
    uint64_t ptr = (uint64_t)handler;

    idt[vector].offset_low16 = (uint16_t)ptr;
    idt[vector].selector = 0x28; 
    idt[vector].ist = 0; 
    idt[vector].flags = flags;
    idt[vector].offset_mid16 = (uint16_t)(ptr >> 16);
    idt[vector].offset_high32 = (uint32_t)(ptr >> 32);
    idt[vector].reserved = 0;
}

static void exception_handler(uint32_t vec) {
    printf("\n%sEXCEPTION RECIVED: %s\n",cRED,exception_names[vec]);
    __asm__ volatile ("cli; hlt"); 
}

static void isr_generic() {
    printf("Something\n");
}

void idt_init(){
    idtr = (idtptr_t) {
        .limit = sizeof(idt) - 1,
        .base = (uint64_t)idt
    };

    for(uint64_t i = 0; i < 32; i++) { 
        encode_idt_entry(i, isr_stubs[i], 0x8e);
        isr[i] = exception_handler;
    }

    for(uint64_t i = 32; i < 256; i++) { 
        encode_idt_entry(i, isr_stubs[i], 0x8e);
        isr[i] = isr_generic;
    }
    idt_load();
    printf("IDT initialized.\n"); 
}
