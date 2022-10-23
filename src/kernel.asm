use32
jmp kernel_entry

times 510 db 0

kernel_entry:
    mov eax, 0xb8000
    ; move cursor by 24 lines
    mov ebx, 24*80*2
    add eax, ebx

    mov ebx, 0x044b
    mov [eax], ebx
    add eax, 2

    mov ebx, 0x0445
    mov [eax], ebx
    add eax, 2
    
    mov ebx, 0x0452
    mov [eax], ebx
    add eax, 2

    mov ebx, 0x044e
    mov [eax], ebx
    add eax, 2

    mov ebx, 0x0445
    mov [eax], ebx
    add eax, 2

    mov ebx, 0x044c
    mov [eax], ebx
    add eax, 2
    ret

    jmp $ ; halt forever