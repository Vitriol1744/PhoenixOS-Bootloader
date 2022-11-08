%include 'arch/x86/cpu_mode.inc'

global vbe_get_controller_info
vbe_get_controller_info:
    enter 0, 0

    push ebx
    push edi
    push es
    GoRealMode
    mov ax, 0x4f00
    LinearToSegOffset [bp + 8], es, edi, di
    int 0x10

    mov ebx, 0
    cmp ax, 0x004f
    jne .done
    mov ebx, 1
.done:
    push ebx
    GoProtectedMode
    pop eax
    pop es
    pop edi
    pop ebx

    leave
    ret

global vbe_get_mode_info
vbe_get_mode_info:
    enter 0, 0

    push ecx
    push ebp
    push es
    push edi
    GoRealMode
    mov ax, 0x4f01
    mov cx, [bp + 8]
    LinearToSegOffset [bp + 12], es, edi, di
    int 0x10

    mov ebx, 0
    cmp ax, 0x004f
    jne .done
    mov ebx, 1
.done:
    push ebx
    GoProtectedMode
    pop eax
    pop edi
    pop es
    pop ebp
    pop ecx

    leave
    ret

global vbe_set_video_mode
vbe_set_video_mode:
    enter 0, 0

    push ecx
    push ebp
    GoRealMode
    mov ax, 0x4f02
    mov bx, [bp + 8]
    int 0x10

    mov ebx, 0
    cmp ax, 0x004f
    jne .done
    mov ebx, 1
.done:
    push ebx
    GoProtectedMode
    pop eax
    pop ebp
    pop ecx

    leave
    ret
