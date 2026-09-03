/* Reset an object-data state and enqueue its asynchronous load callback. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleObjectData_QueueLoad, "ax", %progbits
.balign 4

.extern BattleTaskQueue_Enqueue
.extern func_ov002_02089c78

.global BattleObjectData_QueueLoad
.type BattleObjectData_QueueLoad, %function
BattleObjectData_QueueLoad:
    mov r3, r0
    str r1, [r3, #0x24]
    mov r0, #0
    str r0, [r3, #8]
    ldr r1, [r3, #0x2c]
    ldr ip, .L_task_enqueue
    orr r1, r1, #0x40000000
    str r1, [r3, #0x2c]
    ldr r2, [r3, #0x2c]
    ldr r0, .L_load_callback
    bic r2, r2, #0x20000000
    mov r1, r3
    str r2, [r3, #0x2c]
    bx ip
.L_task_enqueue:
    .word BattleTaskQueue_Enqueue
.L_load_callback:
    .word func_ov002_02089c78
.size BattleObjectData_QueueLoad, . - BattleObjectData_QueueLoad
