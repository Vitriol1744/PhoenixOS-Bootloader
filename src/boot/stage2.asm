bits 16
org 0x20000

jmp stage2_main
%include 'a20.asm'
%include 'gdt.asm'

stage2_main:
    ; initialize segment registers
    mov ax, 0x2000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x0000

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
    bits 32
    mov eax, 0xdeadbeef
    jmp $

bits 16

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

stage2_main_str: db "Jumped into stage2.", endl, 0x0
a20_success_str: db "Successfully enabled a20 gate.", endl, 0x0
a20_failure_str: db "Failed to enable a20 gate!", endl, 0x0

times 1337 db 0x41