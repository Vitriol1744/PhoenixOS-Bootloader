%macro GoRealMode 0
    jmp word 0x18:.entry16

use16
.idt_real:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dd 0			; bios ivt
 
.entry16:
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    ; set code segment
	jmp word 0x00:.real_mode_entry
 
.real_mode_entry:
    mov ax, 0
    mov ds, ax
    mov ss, ax

	lidt [.idt_real]
	sti			; Restore interrupts -- be careful, unhandled int's will kill it.
%endmacro


%macro GoProtectedMode 0
    cli

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 08h:.pmode


.pmode:
    use32
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

%endmacro

; Segment = linear >> 4   - (top 16 bits)
; Offset  = linear & 0x0f - (low 4 bits)
%macro LinearToSegOffset 4
    mov %3, %1
    shr %3, 4
    mov %2, %4
    mov %3, %1
    and %3, 0xf
%endmacro