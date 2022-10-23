%include 'cpu_mode.inc'

%macro LinearToSegOffset 4

    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf

%endmacro

global get_drive_parameters
get_drive_parameters:
    ; save stack frame
    push ebp
    mov ebp, esp

    GO_REAL_MODE

    ; save non-scratch registers
    push es
    push bx
    push esi
    push di

    mov dl, [bp + 8]
    mov ah, 0x08
    mov di, 0
    mov es, di
    stc
    int 0x13

    mov eax, 1
    sbb eax, 0

    LinearToSegOffset [bp + 12], es, esi, si
    mov [es:si], bl

    mov bl, ch
    mov bh, cl
    shr bh, 6
    inc bx

    LinearToSegOffset [bp + 16], es, esi, si
    mov [es:si], bx

    xor ch, ch
    and cl, 0x3f

    LinearToSegOffset [bp + 20], es, esi, si
    mov [es:si], cx

    mov cl, dh
    inc cx

    LinearToSegOffset [bp + 24], es, esi, si
    mov [es:si], cx

    ; load non-scratch registers
    pop di
    pop esi
    pop bx
    pop es

    ; return value
    push eax
    GO_PROTECTED_MODE
    pop eax

    ; restore stack frame
    mov esp, ebp
    pop ebp
    ret

global read_sectors
read_sectors:
    push ebp             
    mov ebp, esp

    GO_REAL_MODE

    push ebx
    push es

    mov dl, [bp + 8] ; drive number

    mov ch, [bp + 12]    ; cylinder (lower 8 bits)
    mov cl, [bp + 13]    ; cylinder (6-7 bits)
    shl cl, 6
    
    mov al, [bp + 16]    ; sector (0-5 bits)
    and al, 0x3f
    or cl, al

    mov dh, [bp + 20]   ; head

    mov al, [bp + 24]   ; number of sectors to read

    LinearToSegOffset [bp + 28], es, ebx, bx

    mov ah, 0x02
    stc
    int 0x13

    ; set return value
    mov eax, 1
    sbb eax, 0

    ; restore regs
    pop es
    pop ebx

    push eax

    GO_PROTECTED_MODE

    pop eax

    mov esp, ebp
    pop ebp
    ret