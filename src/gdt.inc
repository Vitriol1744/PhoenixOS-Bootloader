gdtr:
    dw .end - .start - 1
    dd .start

.start:
    dq 0

    ; 32-bit code segment
    dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
    dw 0                        ; base (bits 0-15) = 0x0
    db 0                        ; base (bits 16-23)
    db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
    db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
    db 0                        ; base high

    ; 32-bit data segment
    dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF for full 32-bit range
    dw 0                        ; base (bits 0-15) = 0x0
    db 0                        ; base (bits 16-23)
    db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
    db 11001111b                ; granularity (4k pages, 32-bit pmode) + limit (bits 16-19)
    db 0                        ; base high

    ; 16-bit code segment
    dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF
    dw 0                        ; base (bits 0-15) = 0x0
    db 0                        ; base (bits 16-23)
    db 10011010b                ; access (present, ring 0, code segment, executable, direction 0, readable)
    db 00001111b                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
    db 0                        ; base high

    ; 16-bit data segment
    dw 0FFFFh                   ; limit (bits 0-15) = 0xFFFFF
    dw 0                        ; base (bits 0-15) = 0x0
    db 0                        ; base (bits 16-23)
    db 10010010b                ; access (present, ring 0, data segment, executable, direction 0, writable)
    db 00001111b                ; granularity (1b pages, 16-bit pmode) + limit (bits 16-19)
    db 0                        ; base high
    ; ; null segment
    ; dq 0

    ; ; Code segment
    ; ; dd 0xffff  ; segment limit
    ; ; dd (10 << 8) | (1 << 12) | (1 << 15) | (0xf << 16) | (1 << 22) | (1 << 23)

    ; ; Data segment
    ; ; dd 0xffff  ; segment limit
    ; ; dd (2 << 8) | (1 << 12) | (1 << 15) | (0xf << 16) | (1 << 22) | (1 << 23)

    ; ; limit
    ; dw 0xffff
    ; ; base low - 16 bits
    ; dw 0x0000
    ; ; base middle - 8 bits
    ; db 0x00
    ; ; access
    ; db 10011010b
    ; ; granularity
    ; db 11001111b
    ; ; base high - 8 bits
    ; db 0x00

    ; ; limit
    ; dw 0xffff
    ; ; base low - 16 bits
    ; dw 0x0000
    ; ; base middle - 8 bits
    ; db 0x00
    ; ; access
    ; db 10010010b
    ; ; granularity
    ; db 11001111b
    ; ; base high - 8 bits
    ; db 0x00
.end:
