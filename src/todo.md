    ; Check presence of PCI, CPUID, MSRs
    ; Load GDTR
    ; Inform BIOS of target processor mode
    ; Get memory map from BIOS
    ; Allocate memory to load kernel image
    ; Load kernel image into buffer
    ; Enable graphics mode
    ; Check kernel image ELF headers
    ; Enable long mode, if 64-bit
    ; Allocate and map memory for kernel segments
    ; Setup stack
    ; Setup COM serial output port
    ; Setup IDT
    ; Disable PIC
    ; Check presence of CPU features (NX, SMEP, x87, PCID, global pages, TCE, WP, MMX, SSE, SYSCALL), and enable them
    ; Assign a PAT to write combining
    ; Setup FS/GS base
    ; Load IDTR
    ; Enable APIC and setup using information in ACPI tables
    ; Setup GDT and TSS 