; this function requires interrupts to be disabled
; OUT:
; ax: 1 if a20 is enabled, 0 otherwise
; AFFECTS: es, ds, di, si
%include 'print.inc'

enable_a20:    
    call .keyboard_controller
    call .check
    cmp ax, 1
    je .exit
    call .bios_a20
    call .check
    cmp ax, 1
    je .exit
    call .fast_a20_gate
    call .check
.exit:
    ret

.keyboard_controller:
    call .kb_wait1
    mov al, 0xad
    out 0x64, al

    call .kb_wait1
    mov al, 0xd0
    out 0x64, al

    call .kb_wait2
    in al, 0x60
    push eax

    call .kb_wait1
    mov al, 0xd1
    out 0x64, al

    call .kb_wait1
    pop eax
    or al, 2
    out 0x60, al

    call .kb_wait1
    mov al, 0xae
    out 0x64, al

    call .kb_wait1
    ret

.bios_a20:
    mov ax, 0x2403
    int 0x15
    jb .int_15ns
    cmp ah, 0
    jnz .int_15ns

    mov ax, 0x2402
    int 0x15
    jb .bios_a20_failed
    cmp ah, 0
    jnz .bios_a20_failed

    cmp al, 1
    jz .a20_bios_exit

    mov ax, 0x2401
    int 0x15
    jb .bios_a20_failed
    cmp ah, 0
    jnz .bios_a20_failed
; int 0x15 is not supported
.int_15ns:
    mov si, .int_15ns_str
    call print_string
    jmp .a20_bios_exit
.bios_a20_failed:
    mov si, .bios_a20_failure_str
    call print_string
.a20_bios_exit:
    ret

.int_15ns_str: db "[BOOT]: INT 0x15 extension is not supported!", endl, 0x0
.bios_a20_failure_str: db "[BOOT]: Failed to activate a20 gate using bios interrupt!", endl, 0x0

.fast_a20_gate:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret
.kb_wait1:
    in al, 0x64
    test al, 2
    jnz .kb_wait1
    ret
.kb_wait2:
    in al, 0x64
    test al, 1
    jz .kb_wait2
    ret

.check:
    pushf
    push ds
    push es
    push di
    push si

    cli
    
    xor ax, ax
    mov es, ax

    not ax
    mov ds, ax

    mov di, 0x0500
    mov si, 0x0510

    mov al, byte[es:di]
    push ax

    mov al, byte[ds:si]
    push ax

    mov byte[es:di], 0x00
    mov byte[ds:si], 0xff
    cmp byte[es:di], 0xff

    pop ax
    mov byte[ds:si], al

    pop ax
    mov byte[es:di], al

    mov ax, 0
    je .check_exit
    mov ax, 1

.check_exit:
    pop si
    pop di
    pop es
    pop ds
    popf

    ret