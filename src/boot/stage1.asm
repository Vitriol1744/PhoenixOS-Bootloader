bits 16
org 0x7c00

jmp 0000:start

%include 'print.asm'

start:
    ; initialize segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; initialize stack
    mov sp, 0x7c00

    ; save disk number
    mov [disk], dl

    ; print bootloader name
    mov si, bootloader_name
    call print_string
        
    mov si, disk_str
    call print_string

    mov al, dl
    call print_num
    mov al, 0xd
    call print_char
    mov al, 0xa
    call print_char

    ; print Hello World
    mov si, string
    call print_string
    ; halt
    jmp $

%define endl 0xd, 0xa

disk_str: db "Loading kernel from disk ", 0x0
bootloader_name: db "PhoenixOS Bootloader", endl, 0x0
string: db "Hello, World!", endl, 0x0
disk: db 0

times 510-($-$$) db 0
dw 0xaa55

