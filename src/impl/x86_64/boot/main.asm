global _start
extern long_mode_start

;wiki.osdev.org

section .text
bits 32
_start:
    mov esp, stack_top                  ;initialize the stack pointer (no valid data in the stack yet)

    call check_multiboot                ;confirms we were loaded by a multiboot2 loader, doesn't modify stack or registers
    call check_cpuid                    ;ensures that cpu flags can be retrieved
    call check_long_mode                ;checks to make sure that long mode is available

    call setup_page_tables
    call enable_paging
    call enable_interupts               ;not sure if other steps are required before this or not, not implemented yet

    lgdt [gdt64.pointer]
    ;lidt [idt64.pointer]                ;don't know if this is correct
    jmp gdt64.code_segment:long_mode_start  ;jump into main64.asm, set up segment regs and call the kernel_main entrypoint

    hlt

check_multiboot:
    cmp eax, 0x36d76289                 ;a magic number eax should have that signifies multiboot or not
    jne .no_multiboot                   ;jump into subroutine to deal with the error
    ret
.no_multiboot:
    mov al, 'M'                         ;Error code to display in tui
    jmp error                           ;  Displays contents of al

;attempt to flip cpuid flag in EFLAGS and if it can flip cpuid is available
check_cpuid:
                            ;Modifying the eflags to enable cpu features
    pushfd                  ;  move eflags to stack
    pop eax                 ;  save eflags from stack to ecx
    mov ecx, eax            ;  ..
    xor eax, 1 << 21        ;  attempt to flip cpuid bit (bit #21)
    push eax                ;save modified flags to stack
    popfd                   ;pop modified flags into the eflags reg
    pushfd                  ;save our modified flags 
    pop eax                 ;  pop flags into eax, ecx contains the original eflag bits
    push ecx                ;push the saved flags back onto the stack
    popfd                   ;pop the saved flags back into eflags
    cmp eax, ecx            ; if the overal value of eax and ecx are still the same, the cpuid bit failed to flip. therefore we have no cpuid capability 
    je .no_cpuid            ;
    ret
.no_cpuid:
    mov al, 'C'
    jmp error

;check if the cpu supports extended processor info using cpuid
;   requires to have checked for cpuid prior to calling
;https://wiki.osdev.org/CPUID
;https://wiki.osdev.org/Setting_Up_Long_Mode
;longmode essentially unlocks extra cpu functionality
;   - 64 bit registers (rax, rbx, etc)
;   - 
check_long_mode:
    mov eax, 0x80000000         ;'intel-extended' code for cpuid
    cpuid                       ;uses eax as arg, returns a value greater than the magic number if
                                ; the processor supports extended info
    cmp eax, 0x80000001         ;if eax is indeed greaater than 0x80000000 we support longmode
    jb .no_long_mode            ;if not longmode is not supported

    mov eax, 0x80000001         ;'intel-features' code for cpuid
    cpuid                       ;sotres a value into edx based on eax input
    test edx, 1 << 29           ;test bit 29 of our 32-bit edx cpuid output, the name of this bit is 'CPUID_FEAT_EDX_TM'. Why?
    jz .no_long_mode            ;if bit 29 is zero long mode is not available
    ret
.no_long_mode:
    mov al, 'L'
    jmp error

; I don't know enough about paging to understand this code
setup_page_tables:
    ;identity mapping
    mov eax, page_table_l3      ;move the address of the level3 page table to eax (but why would you bitwise or a page table address?)
    or eax, 11b                 ;present | writable
    mov [page_table_l4], eax

    mov eax, page_table_l2
    or eax, 11b                 ;present | writable
    mov [page_table_l3], eax    ;

    mov ecx, 0      ;counter
.loop:
    mov eax, 0x200000 ; MiB
    mul ecx
    or eax, 10000011b   ;huge_page | present | writable
    mov [page_table_l2 + ecx * 8], eax
    inc ecx         ;increment counter
    cmp ecx, 512    ;tells us weather the whole table is mapped
    jne .loop
    ret

enable_paging:
    ;pass the beginning of the page table to cpu
    mov eax, page_table_l4
    mov cr3, eax

    ;enable PAE flag from cr4 reg
    ;   PAE mode enables the page map to reference up to 36-bit page addresses
    mov eax, cr4        ;cr4 contains cpu configuration flags
    or eax, 1 << 5      ;bit 5 of cr4 is PAE 'physical address extension'. we turn that on. When it is off paging supports 32bit addresses, when it is on paging supports up to 64bit addresses
    mov cr4, eax

    ; enable long mode
    mov ecx, 0xC0000080
    ;EDX:EAX ← MSR[ECX];   https://www.felixcloutier.com/x86/rdmsr
    ;A bit to note is bit 0 (System Call Enable SCE)
    rdmsr               ; This is modifying the 64-bit Model Scecific Register (MSR), uses eax AND edx due to needing 64 bits of total capacity
    or eax, 1 << 8      ;    turns on the 8th bit of the eax reg    (bit 8 of eax is for long-mode-enable, we never actually enabled long mode in `check_long_mode`)
                        ;    switches from real mode into protected mode
    wrmsr               ; Writes the eax and edx back into the 64-bit MSR

    ; enable paging
    mov eax, cr0        ;cr0 is a control register that outlines some basic settings for the cpu
    or eax, 1 << 31     ;bit 31 is paging, or-ing enables paging
    mov cr0, eax
    
    ret

enable_interupts:
    ret

;error serves as a way out of this initialization assembly file
error:
    ; print "ERR: X" where X is the error code
    ; vga video memory starts at 0xb8000
    mov dword [0xb8000], 0x4f524f45     ;'E'
    mov dword [0xb8004], 0x4f3a4f52     ;'R'
    mov dword [0xb8008], 0x4f204f20
    mov byte  [0xb800a], al             ;program defined byte
    hlt                                 ;termination command

section .bss
align 4096
page_table_l4: ;level 4 page table
    resb 4096
page_table_l3: ;level 3 page table
    resb 4096
page_table_l2: ;level 2 page table
    resb 4096

stack_bottom:
    resb 4096 * 4       ;16,384 bytes
stack_top:

;read only data
section .rodata
gdt64:      ;global descriptor table
    dq 0    ;zero entry

.code_segment: equ $ - gdt64
    ;  exec flag                present     64 bit
    dq (1 << 43) |  (1 << 44) | (1 << 47) | (1 << 53) ;code segment
.pointer:
    dw $-gdt64-1
    dq gdt64

