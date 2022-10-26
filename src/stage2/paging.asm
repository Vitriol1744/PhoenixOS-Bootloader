global load_page_directory
load_page_directory:
    enter 0, 0
    mov eax, [esp + 8]
    mov cr3, eax
    leave
    ret

global enable_paging
enable_paging:
    enter 0, 0
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    leave
    ret