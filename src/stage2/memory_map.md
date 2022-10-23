; 0x00000000 	0x000003FF 	1 KiB 	Real Mode IVT (Interrupt Vector Table) 	unusable in real mode 	640 KiB RAM ("Low memory")
; 0x00000400 	0x000004FF 	256 bytes 	BDA (BIOS data area)
; 0x00000500 	0x00007BFF 	almost 30 KiB 	Conventional memory 	usable memory
; 0x00007C00 	0x00007DFF 	512 bytes 	boot_sector(stage1)
; 0x00007E00 	0x0007FFFF 	480.5 KiB 	Conventional memory <-- Somewhere here we will put stage2
; 0x00080000 	0x0009FFFF 	128 KiB 	EBDA (Extended BIOS Data Area) 	partially used by the EBDA
; 0x000A0000 	0x000BFFFF 	128 KiB 	Video display memory 	hardware mapped 	384 KiB System / Reserved ("Upper Memory")
; 0x000C0000 	0x000C7FFF 	32 KiB (typically) 	Video BIOS 	ROM and hardware mapped / Shadow RAM
; 0x000C8000 	0x000EFFFF 	160 KiB (typically) 	BIOS Expansions
; 0x000F0000 	0x000FFFFF 	64 KiB 	Motherboard BIOS 