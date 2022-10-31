use32
%include 'cpu_mode.inc'

%macro LinearToSegOffset 4

    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf

%endmacro
global mmap_entry_count
mmap_entry_count: dw 0
old_stack: dw 0

global get_memory_map
get_memory_map:
     ; save stack frame
    push ebp
    mov ebp, esp

    GoRealMode

    ; save non-scratch registers
    push es
    push bx
    push esi
    push di
    mov [old_stack], bp

    LinearToSegOffset [bp + 8], es, edi, di

    xor bp, bp
    xor ebx, ebx
    mov eax, 0xe820                 ; we want to get the memory map
    mov edx, 0x0534D4150            ; 'SMAP'
    mov ecx, 0x18                   ; sizeof(SMAP_entry_t)
    mov dword es:[di + 20], 1	    ; force a valid ACPI 3.X entry
    int 0x15
    jc failed
    mov edx, 0x0534D4150 
    cmp eax, edx
    jne failed
    test ebx, ebx
    je failed
    jmp jmpin

e820call:
    mov eax, 0xe820                 ; we want to get the memory map
    mov edx, 0x0534D4150            ; 'SMAP'
    mov ecx, 0x18                   ; sizeof(SMAP_entry_t)
    mov dword es:[di + 20], 1	    ; force a valid ACPI 3.X entry
    int 0x15
    jc exit
    mov edx, 0x0534D4150

jmpin:
    jcxz skipentry
    cmp cl, 0x14
    jbe next
    test byte [es:di + 0x14], 1
    je skipentry

next:
    mov ecx, [es:di + 0x8]
    or ecx, [es:di + 0xc]
    jz skipentry
    inc bp
    add di, 24

skipentry:
    test ebx, ebx
    jne e820call

exit:
    mov [mmap_entry_count], bp
    mov bp, [old_stack]
    cli

    ; load non-scratch registers
    pop di
    pop esi
    pop bx
    pop es

    ; return value
    push eax
    GoProtectedMode
    pop eax

    ; restore stack frame
    mov esp, ebp
    pop ebp
    ret


failed:
    pop bp
    pop di
    pop esi
    pop bx
    pop es

    ; return value
    push 1
    GoProtectedMode
    pop eax

    int 0x3 ; We'll just crash if we fail. Afterall how will we continue without a memory map
