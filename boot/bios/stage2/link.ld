OUTPUT_FORMAT(binary)
STARTUP(bin/main.o)
SECTIONS{
    .entry : ALIGN(0x1000){
        *(.entry)
    }
    .text : ALIGN(0x1000){
        *(.text)
    }
    .data : ALIGN(0x1000){
        *(.data)
    }
    .rodata : ALIGN(0x1000){
        *(.rodata)
    }
    .bss : ALIGN(0x1000){
        *(COMMON)
        *(.bss)
    }
}