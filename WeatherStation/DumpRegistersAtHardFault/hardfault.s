/* XXX: Uncomment!!!
; Reference: http://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html

; On the Cortex-M3, the processor core automatically saves some interesting
; registers to the stack on initial exception entry. The following
; HardFault_Handler is a short assembly function to determine which stack (MSP
; or PSP) was being used when the fault occurred; once this is done, the fault
; handler assembly code passes a pointer to the stack frame into a C function
; called prvGetRegistersFromStack() which then aids user-debugging.

    AREA asm_func, CODE, READONLY

; Export HardFault_Handler function location so that linker can find it
    EXPORT HardFault_Handler
HardFault_Handler

    TST LR, #4 ; test bit 2 of the EXC_RETURN placed in LR to determine which
               ; stack was in use before entering the handler

    ; move the appropriate stack pointer value into R0
    ITE EQ
    MRSEQ R0, MSP
    MRSNE R0, PSP

    LDR R1, [R0, #24] ; get stacked PC

    ; call prvGetRegistersFromStack() with R0 set as the stack pointer at the
    ; time of exception entry.
    LDR R2, HANDLER2_ADDRESS_CONST
    BX R2
    IMPORT prvGetRegistersFromStack
    ALIGN 4
HANDLER2_ADDRESS_CONST DCD prvGetRegistersFromStack

    END
*/
