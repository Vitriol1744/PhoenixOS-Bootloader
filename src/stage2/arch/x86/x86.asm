use32
%include 'arch/x86/cpu_mode.inc'

global disk_reset
disk_reset:
    enter 0, 0
    GoRealMode

    mov ah, 0
    ; drive number
    mov dl, [bp + 8]
    stc
    int 0x13

    ; return value
    mov eax, 1
    sbb eax, 0

    push eax
    GoProtectedMode
    pop eax

    leave
    ret

global get_drive_parameters
get_drive_parameters:
    enter 0, 0
    GoRealMode

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
    GoProtectedMode
    pop eax

    leave
    ret

global read_sectors
read_sectors:
    enter 0, 0

    GoRealMode

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
    push eax

    mov eax, 0
    jc .done

    ; if successful, set return value to the number of sectors transferred
    pop eax
.done:
    ; restore regs
    pop es
    pop ebx

    push eax

    GoProtectedMode

    pop eax

    leave
    ret

global e820_get_mmap_entry
e820_get_mmap_entry:
    enter 0, 0

    push ebx
    push ecx
    push edx
    push esi
    push edi
    push ds
    push es
    GoRealMode

    ; buffer for result
    LinearToSegOffset [bp + 8], es, edi, di
    ; continuation id
    LinearToSegOffset [bp + 12], ds, esi, si

    mov eax, 0xe820
    ; 'SMAP' signature
    mov edx, 0x534d4150
    ; continuation id
    mov ebx, ds:[si]
    ; size of buffer
    mov ecx, 24
    int 0x15

    ; carry set on error
    jnc .error
    ; eax should be 'SMAP'
    cmp eax, 0x534d4150
    jne .error
    ; set continuation id
    mov ds:[si], ebx
    jmp .done
.error:
    ; return value
    xor ecx, ecx
.done:
    push ecx
    GoProtectedMode
    pop eax
    pop es
    pop ds
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx

    leave
    ret