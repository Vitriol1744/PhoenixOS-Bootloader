use16
org 0x7c00

%define STAGE2_SEGMENT 0x0000
%define STAGE2_OFFSET 0x7e00

jmp short skip_bpb
nop

; bios parameter block
times 0x03-($ - $$) db 0
bpb_oem_identifier: dq 0
bpb_bytes_per_sector: dw 512
bpb_sectors_per_cluster: db 0
bpb_reserved_sector_count: dw 0
bpb_fat_count: db 0
bpb_root_directory_entry_count: dw 0
bpb_total_sector_count: dw 0
bpb_media_descriptor_type: db 0
bpb_sectors_per_fat: dw 0
bpb_sectors_per_track: dw 0
bpb_head_count: dw 0
bpb_hidden_sector_count: dd 0
bpb_large_sector_count: dd 0
; extended boot record
ebr_drive_number: db 0
ebr_reserved: db 0
ebr_signature: db 0
ebr_volume_id: dd 0
ebr_volume_label: db 'PhoenixBoot' ; 11 bytes
ebr_system_id: dq 0

skip_bpb:
    jmp 0000:start
    nop
start:
    cli
    cld

    ; initialize segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; initialize stack
    mov sp, 0x7c00

    ; save disk number
    mov [disk], dl

    ; print bootloader name
    mov si, bootloader_name
    call print_string
    
    mov si, loading_stage2_str
    call print_string

    mov bh, dl
    ;TODO: call print_num
    mov al, 0xd
    call print_char
    mov al, 0xa
    call print_char

    ; load stage2 into memory
load_stage2:
    ; ES:BX - Data buffer where stage2 will be loaded
    mov ax, STAGE2_SEGMENT
    mov es, ax
    mov bx, STAGE2_OFFSET
    
    mov ah, 0x02    ; read sectors
    mov al, 3       ; number of sectors to read
    mov al, stage2_size / 512 + 1
    mov ch, 0       ; cylinder number
    mov cl, 2       ; sector number
    mov dh, 0       ; head number
    mov dl, [disk]  ; drive number
    mov di, 3

.disk_read:
    pusha
    ;TODO: Check for 0x13 extension support
    int 0x13
    popa
    jnc .done

    call .disk_reset
    dec di
    test di, di
    jnz .disk_read

.disk_reset:
    mov ah, 0x00
    int 0x13
    jc .error
    ret

; carry flag set on error
.error:
    mov si, load_stage2_failure_str
    call print_string
    jmp on_error
.done:
    mov si, load_stage2_success_str
    call print_string
    jmp word STAGE2_SEGMENT:STAGE2_OFFSET
    nop ; should not get here

on_error:
    mov ah, 0x00
    int 0x16 ; wait for keypress
    jmp 0xffff:0 ; reboot

%include 'print.asm'

loading_stage2_str: db "[BOOT]: Loading stage2 from disk ", 0x0
bootloader_name: db "[BOOT]: PhoenixOS Bootloader - Stage One", endl, 0x0
load_stage2_failure_str: db "[BOOT]: Failed to load stage2", endl, 0x0
load_stage2_success_str: db "[BOOT]: Successfully loaded stage2 at 0x0000:0x7e00", endl, 0x0
disk: db 0

%if ($ - $$) > 510
    %fatal 'Bootloader code exceed 510 bytes!'
%endif

times 510-($ - $$) db 0
dw 0xaa55

stage2_bin_start:
incbin 'stage2.bin'
stage2_bin_end:
stage2_size equ (stage2_bin_end - stage2_bin_start)