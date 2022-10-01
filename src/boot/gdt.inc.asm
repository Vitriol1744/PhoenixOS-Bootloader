gdtr:
    dw .end - .start - 1
    dd .start

.start:
    ; null segment
    dq 0

    ; limit
    dw 0xffff
    ; base low - 16 bits
    dw 0x0000
    ; base middle - 8 bits
    db 0x00
    ; access
    db 10011010b
    ; granularity
    db 11001111b
    ; base high - 8 bits
    db 0x00

    ; limit
    dw 0xffff
    ; base low - 16 bits
    dw 0x0000
    ; base middle - 8 bits
    db 0x00
    ; access
    db 10010010b
    ; granularity
    db 11001111b
    ; base high - 8 bits
    db 0x00
.end: