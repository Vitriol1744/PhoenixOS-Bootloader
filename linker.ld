ENTRY(stage2_entry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x7e00;

    .entry              : { __entry_start = .;      *(.entry)   }
    .text               : { __text_start = .;       *(.text)    }
    .data               : { __data_start = .;       *(.data)    }    
    .rodata             : { __rodata_start = .;     *(.rodata)  }    
    .bss                : { __bss_start = .;        *(.bss);    }    __bss_end = .;
}