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

print_char:
    pusha
    mov ah, 0x0e ; bios print
    int 0x10
    popa
    ret