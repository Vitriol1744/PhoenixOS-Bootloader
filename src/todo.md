    ; Check presence of PCI, CPUID, MSRs
    ; Inform BIOS of target processor mode
    ; Get memory map from BIOS
    ; Setup COM serial output port
    ; Setup IDT
    ; Check presence of CPU features (NX, SMEP, x87, PCID, global pages, TCE, WP, MMX, SSE, SYSCALL), and enable them
    ; Assign a PAT to write combining
    ; Setup FS/GS base
    ; Load IDTR
    ; Enable APIC and setup using information in ACPI tables
    ; Setup GDT and TSS 