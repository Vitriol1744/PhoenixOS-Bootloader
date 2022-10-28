use16

section .entry
    jmp stage2_entry
global stage2_entry
extern stage2_main
section .text
%include 'cpu_mode.inc'

stage2_entry:
    ; initialize segment registers
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    mov bp, sp

    ; print new line character
    mov al, 0xd
    call print_char
    mov al, 0xa
    call print_char

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
 
    push dx
    call stage2_main
    mov eax, 0xb8000
    mov ebx, 0x0441
    mov [eax], ebx
    jmp $ ; halt forever

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

%include 'a20.inc'
%include 'gdt.inc'

stage2_main_str: db "[BOOT]: PhoenixOS Bootloader - Stage Two", endl, 0x0
a20_success_str: db "[BOOT]: Successfully enabled a20 gate.", endl, 0x0
a20_failure_str: db "[BOOT]: Failed to enable a20 gate!", endl, 0x0