use32

global go_long_mode
go_long_mode:
    call enable_paging
    lgdt [gdt64.pointer]

    pop ebx ; discard return address
    jmp gdt64.code:long_mode_start

enable_paging:
    ; enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the EFER MSR
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

use64
long_mode_start:
    ; set up segment registers
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; load kernel entrypoint
    pop rbx
    pop rax
    mov rdi, rax
    ; jump to kernel
    call rbx

section .rodata
; 64 bit GDT
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64 ; code selector
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53)
.pointer:
    dw $ - gdt64 - 1
    dq gdt64