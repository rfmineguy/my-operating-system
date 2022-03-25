global long_mode_start
extern kernel_main

section .text
bits 64
long_mode_start:
    mov ax, 0
    mov ss, ax      ;load 0 into data segment registers  
    mov ds, ax      ;
    mov es, ax      ;
    mov fs, ax      ;
    mov gs, ax      ;

    ;print 'OK'
    call start_kernel 
    ;mov dword [0xb8000], 0x2f4b2f4f
    hlt

start_kernel:
    call kernel_main
    ret
