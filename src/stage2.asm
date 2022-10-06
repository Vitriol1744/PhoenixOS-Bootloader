use16

section .entry
    jmp stage2_entry
global stage2_entry
extern _start
section .text
stage2_entry:
    ; initialize segment registers
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    mov bp, sp

    mov si, stage2_main_str
    call print_string
    call enable_a20
    cmp ax, 1
    jne a20_failure
    call a20_activated

switch_to_protected_mode:
    cli
    lgdt [gdtr]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp dword 0x08:(protected_mode_main)

protected_mode_main:
    use32
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov eax, 0xb8000
    mov ebx, [letter]
    ;mov [eax], ebx
    jmp _start

letter: db 'A'

use16
a20_activated:
    mov si, a20_success_str
    call print_string
    ret

a20_failure:
    mov si, a20_failure_str
    call print_string
    jmp on_failure

on_failure:
    mov eax, 0xdeadbeef
    jmp $

%include 'a20.asm'
%include 'gdt.asm'

stage2_main_str: db "[BOOT]: PhoenixOS Bootloader - Stage Two", endl, 0x0
a20_success_str: db "[BOOT]: Successfully enabled a20 gate.", endl, 0x0
a20_failure_str: db "[BOOT]: Failed to enable a20 gate!", endl, 0x0