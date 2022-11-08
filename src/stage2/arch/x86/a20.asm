use32

global a20_enable
a20_enable:
    enter 0, 0

    call .wait1
    mov al, 0xad
    out 0x64, al

    call .wait1
    mov al, 0xd0
    out 0x64, al

    call .wait2
    in al, 0x60
    push eax

    call .wait1
    mov al, 0xd1
    out 0x64, al

    call .wait1
    pop eax
    or al, 2
    out 0x60, al

    call .wait1
    mov al, 0xae
    out 0x64, al

    call .wait1
    call a20_is_enabled
    cmp eax, 1
    je .exit
    call fast_a20
    call a20_is_enabled

.exit:
    leave
    ret

.wait1:
    in al, 0x64
    test al, 2
    jnz .wait1
    ret
.wait2:
    in al, 0x64
    test al, 1
    jz .wait2
    ret

fast_a20:
    in al, 0x92
    test al, 2
    jnz .exit
    or al, 2
    and al, 0xfe
    out 0x92, al
.exit:
    ret

global a20_is_enabled
a20_is_enabled:
    enter 0, 0
    pushad

    mov edi, 0x112345
    mov esi, 0x012345
    mov [esi], edi
    mov [edi], edi
    cmpsd

    popad
    ; set return value
    jne a20_enabled
    mov eax, 1
a20_enabled:
    and eax, 1
    leave
    ret