.syntax unified
.text

.global PendSV_Handler
.thumb_func
PendSV_Handler:
    /*  MRS: move contents of 
        special register to a 
        general purpose register */
    MRS R0, PSP
    /*  ARM assembly is beautiful,
        wdym I can do R4-R11 at once,
        also this is STM-DB, where
        DB is decrement before each access */
    STMDB R0!, {R4-R11}

    /* Handle saving PSP */

    /*  Get pointer to current task */
    LDR R1, =current_task
    /* Dereference pointer */
    LDR R1, [R1]
    /*  current_task->stack_ptr = R0 */
    STR R0, [R1, #0]

    /* Get Next Task*/
    BL scheduler_select_next_task

    /* Load new stack_ptr into PSP */
    LDR R0, =current_task
    /* Dereference ptr */
    LDR R0, [R0]
    /* Load stack ptr */
    LDR R0, [R0, #0]
    /* Pop Registers */
    LDMIA R0!, {R4-R11}
    /*  Need to return from
        PSP to thread mode,
        0xFFFFFFFD returns 
        to Thread from PSP */
    MSR PSP, R0

    MOV LR, #0xFFFFFFFD
    BX LR

/*  Function to manually bootstrap task
    loop on scheduler start */
.global port_start_first_task
.thumb_func
port_start_first_task:
    BL scheduler_select_next_task
    SVC #0

/*  Need SVC exception to 
    get into Handler mode */
.global SVC_Handler
.thumb_func
SVC_Handler:
    /* Load new stack_ptr into PSP */
    LDR R0, =current_task
    /* Dereference ptr */
    LDR R0, [R0]
    /* Load stack ptr */
    LDR R0, [R0, #0]

    /* Load Fake Context from stack */

    /*  Load all registers of fake frame*/
    LDMIA R0!, {R4-R11}
    /*  Store updated PSP back */
    MSR PSP, R0

    /* Switch from Thread mode to PSP */

    /*  CONTROl register lets me 
        designate which stack to use */
    MOV R1, #2
    MSR CONTROL, R1
    /* ISB is used to refresh & sync */
    ISB

    /* Return to Thread Mode */

    MOV LR, #0xFFFFFFFD
    BX LR
