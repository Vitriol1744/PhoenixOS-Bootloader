%define endl 0xd, 0xa

print_string:
    pusha
    pushf
.loop:
    lodsb ; load character pointed by si to al
    test al, al
    jz print_string.done
    call print_char
    jmp print_string.loop
.done:
    popf
    popa
    ret

print_num:
    mov cx, 4
.lp:
    mov al, bh
    shr al, 4

    cmp al, 0xa
    jb .below0xa

    add al, 'A' - 0xA - '0'
.below0xa:
    add al, '0'
    call print_char
    shl bx, 4
    loop .lp
    ret

print_char:
    pusha
    mov ah, 0x0e ; bios print
    int 0x10
    popa
    ret