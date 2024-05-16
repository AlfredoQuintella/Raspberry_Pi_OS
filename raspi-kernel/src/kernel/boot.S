//the linker will combine a bunch of files this include to a binary
.section ".text.boot" // signalizes for the linker where to compile this file in the binary 

.global _start // signalizes that other files can use this _start from this file

_start:
    mrc p15, #0, r1, c0, c0, #5
    and r1, r1, #3
    cmp r1, #0
    bne halt //it sends every register that its last 2 bit are not 00 to the halt and this by some reason shuts 3 out of 4 cores down (see end of code)

    mov sp, #0x8000 // C can start from the register 0x8000 to before (after 0x8000 is used for kernel)

    ldr r4, =__bss_start
    ldr r9, =__bss_end //gets start and end of the memory segment that C variables that are not initialized on compile time is used
    mov r5, #0
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b       2f

1:
    stmia r4!, {r5-r8}

2:
    cmp r4, r9
    blo 1b 
    //all of this 3 last segments are just to zero the memory so we can initialize

    ldr r3, =kernel_main
    blx r3 // go to the location of the kernel_main

halt:
    wfe // wait for event (turns into a low-power state till some event happen)
    b halt // if some codes enter here it loops to death