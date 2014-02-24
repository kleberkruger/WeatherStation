#include "mbed.h"
#include <stdint.h>

/* Reference: http://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html */

/*
 * The stack frame of the fault handler contains the state of the ARM Cortex-M
 * registers at the time that the fault occurred. The code below shows how to
 * read the register values from the stack into C variables. Once this is done,
 * the values of the variables can be inspected or written to the console just
 * as an other variable.
 */
extern "C" void
prvGetRegistersFromStack(uint32_t *pulFaultStackAddress)
{
    /* These are volatile to try and prevent the compiler/linker optimising them
     * away as the variables never actually get used.  If the debugger won't
     * show the values of the variables, make them global my moving their
     * declaration outside of this function.*/

    /*
     * Only the ProgramCounter (PC) is useful in this particular case.
     */
    // volatile uint32_t r0;
    // volatile uint32_t r1;
    // volatile uint32_t r2;
    // volatile uint32_t r3;
    // volatile uint32_t r12;
    // volatile uint32_t lr; /* Link register. */
    volatile uint32_t pc; /* Program counter. */
    // volatile uint32_t psr;/* Program status register. */

    // r0 = pulFaultStackAddress[ 0 ];
    // r1 = pulFaultStackAddress[ 1 ];
    // r2 = pulFaultStackAddress[ 2 ];
    // r3 = pulFaultStackAddress[ 3 ];
    // r12 = pulFaultStackAddress[ 4 ];
    // lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    // psr = pulFaultStackAddress[ 7 ];

    error("\r\nHardFault_Handler: from pc = 0x%08x\r\n", (unsigned)pc);
}
