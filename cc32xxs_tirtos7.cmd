/*
 * copyright (c) 2018-2020, texas instruments incorporated
 * all rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * The starting address of the application.  Normally the interrupt vectors
 * must be located at the beginning of the application.
 */
#define SRAM_BASE 0x20004000
#define SRAM0_BASE 0x20000000

--stack_size=1024   /* C stack is also used for ISR stack */

HEAPSIZE = 0x8000;  /* Size of heap buffer used by HeapMem */

MEMORY
{
    SRAM0 (RWX) : origin = 0x20000000, length = 0x4000
    SRAM  (RWX) : origin = 0x20004000, length = 0x00040000 - 0x4000
    /* Explicitly placed off target for the storage of logging data.
     * The data placed here is NOT loaded onto the target device.
     * This is part of 1 GB of external memory from 0x60000000 - 0x9FFFFFFF.
     * ARM memory map can be found here:
     * https://developer.arm.com/documentation/ddi0337/e/memory-map/about-the-memory-map
     */
    LOG_DATA (R) : origin = 0x90000000, length = 0x40000
}

/* Section allocation in memory */

--retain "*(.resetVecs)"

SECTIONS
{
    .resetVecs  : > 0x20004000
    .text       : > SRAM
    .bss        : > SRAM
    .TI.ramfunc : > SRAM
    .data       : > SRAM
    .const      : > SRAM
    .rodata     : > SRAM

    .vecs       : > 0x20000000
    .sysmem     : > SRAM
    .cinit      : > SRAM
    .pinit      : > SRAM
    .init_array : > SRAM

    /* Heap buffer used by HeapMem */
    .priheap    : {
        __primary_heap_start__ = .;
        . += HEAPSIZE;
        __primary_heap_end__ = .;
    } > SRAM align 8

    .stack      : > SRAM0(HIGH)
    .log_data   : > LOG_DATA, type = COPY
}

--symbol_map __TI_STACK_SIZE=__STACK_SIZE
--symbol_map __TI_STACK_BASE=__stack

-u_c_int00
--retain "*(.resetVecs)"
--retain "*(.vecs)"
