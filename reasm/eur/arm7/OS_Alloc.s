/* NitroSDK heap validation, initialization, allocation, and free-list helpers. */

.syntax unified
.arch armv4t
.arm
.section .pit_OS_Alloc, "ax", %progbits
.balign 4

.extern OS_DisableInterrupts
.extern OS_RestoreInterrupts
.extern OSiHeapInfo

.global OS_CheckHeap
.type OS_CheckHeap, %function
OS_CheckHeap:
    push {r4, r5, r6, r7, r8, lr}
    mov r8, r0
    mov r7, r1
    mov r6, #0
    mov r5, r6
    mvn r4, #0
    bl OS_DisableInterrupts
    ldr r1, .L_heap_info_check
    ldr r12, [r1, r8, lsl #2]
    ldr r2, [r12, #0x10]
    cmp r2, #0
    beq .Lcheck_done
    cmp r7, #0
    blt .Lcheck_done
    ldr r1, [r12, #4]
    cmp r7, r1
    bge .Lcheck_done
    mov r1, #0x0C
    mul r1, r7, r1
    add r3, r2, r1
    ldr r2, [r2, r1]
    cmp r2, #0
    blt .Lcheck_done
    ldr r1, [r3, #8]
    cmp r1, #0
    beq .Lcheck_free_test
    ldr r7, [r1]
    cmp r7, #0
    bne .Lcheck_done
    b .Lcheck_free_test
.Lcheck_free_loop:
    ldr r7, [r12, #8]
    cmp r7, r1
    bhi .Lcheck_done
    ldr r7, [r12, #0x0C]
    cmp r1, r7
    bhs .Lcheck_done
    ands r7, r1, #0x1F
    bne .Lcheck_done
    ldr lr, [r1, #4]
    cmp lr, #0
    beq .Lcheck_free_next_link
    ldr r7, [lr]
    cmp r7, r1
    bne .Lcheck_done
.Lcheck_free_next_link:
    ldr r7, [r1, #8]
    cmp r7, #0x40
    blo .Lcheck_done
    ands r1, r7, #0x1F
    bne .Lcheck_done
    add r6, r6, r7
    cmp r6, #0
    ble .Lcheck_done
    cmp r6, r2
    bgt .Lcheck_done
    mov r1, lr
.Lcheck_free_test:
    cmp r1, #0
    bne .Lcheck_free_loop
    ldr r1, [r3, #4]
    cmp r1, #0
    beq .Lcheck_used_test
    ldr r3, [r1]
    cmp r3, #0
    bne .Lcheck_done
    b .Lcheck_used_test
.Lcheck_used_loop:
    ldr r3, [r12, #8]
    cmp r3, r1
    bhi .Lcheck_done
    ldr r3, [r12, #0x0C]
    cmp r1, r3
    bhs .Lcheck_done
    ands r3, r1, #0x1F
    bne .Lcheck_done
    ldr lr, [r1, #4]
    cmp lr, #0
    beq .Lcheck_used_next_link
    ldr r3, [lr]
    cmp r3, r1
    bne .Lcheck_done
.Lcheck_used_next_link:
    ldr r7, [r1, #8]
    cmp r7, #0x40
    blo .Lcheck_done
    ands r3, r7, #0x1F
    bne .Lcheck_done
    cmp lr, #0
    beq .Lcheck_used_nonoverlap
    add r1, r1, r7
    cmp r1, lr
    bhs .Lcheck_done
.Lcheck_used_nonoverlap:
    add r6, r6, r7
    sub r1, r7, #0x20
    add r5, r5, r1
    cmp r6, #0
    ble .Lcheck_done
    cmp r6, r2
    bgt .Lcheck_done
    mov r1, lr
.Lcheck_used_test:
    cmp r1, #0
    bne .Lcheck_used_loop
    cmp r6, r2
    moveq r4, r5
.Lcheck_done:
    bl OS_RestoreInterrupts
    mov r0, r4
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.size OS_CheckHeap, . - OS_CheckHeap
.L_heap_info_check:
    .word OSiHeapInfo

.global OS_CreateHeap
.type OS_CreateHeap, %function
OS_CreateHeap:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r4, r0
    mov r6, r1
    mov r5, r2
    bl OS_DisableInterrupts
    ldr r1, .L_heap_info_create
    ldr r7, [r1, r4, lsl #2]
    add r1, r6, #0x1F
    bic r6, r1, #0x1F
    bic r5, r5, #0x1F
    mov r4, #0
    ldr lr, [r7, #4]
    mov r1, #0x0C
    b .Lcreate_test
.Lcreate_loop:
    ldr r3, [r7, #0x10]
    mul r2, r4, r1
    add r12, r3, r2
    ldr r2, [r3, r2]
    cmp r2, #0
    bge .Lcreate_next
    sub r1, r5, r6
    str r1, [r12]
    mov r2, #0
    str r2, [r6]
    str r2, [r6, #4]
    ldr r1, [r12]
    str r1, [r6, #8]
    str r6, [r12, #4]
    str r2, [r12, #8]
    bl OS_RestoreInterrupts
    mov r0, r4
    b .Lcreate_return
.Lcreate_next:
    add r4, r4, #1
.Lcreate_test:
    cmp r4, lr
    blt .Lcreate_loop
    bl OS_RestoreInterrupts
    mvn r0, #0
.Lcreate_return:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size OS_CreateHeap, . - OS_CreateHeap
.L_heap_info_create:
    .word OSiHeapInfo

.global OS_InitAlloc
.type OS_InitAlloc, %function
OS_InitAlloc:
    push {r4, r5, r6, r7, r8, lr}
    mov r7, r0
    mov r5, r1
    mov r4, r2
    mov r6, r3
    bl OS_DisableInterrupts
    ldr r1, .L_heap_info_init
    str r5, [r1, r7, lsl #2]
    mov r2, #0x0C
    mul r1, r6, r2
    add r3, r5, #0x14
    str r3, [r5, #0x10]
    str r6, [r5, #4]
    mov r8, #0
    mvn lr, #0
    mov r12, r8
    b .Linit_test
.Linit_loop:
    ldr r6, [r5, #0x10]
    mul r3, r8, r2
    add r7, r6, r3
    str lr, [r6, r3]
    str r12, [r7, #8]
    ldr r3, [r7, #8]
    str r3, [r7, #4]
    add r8, r8, #1
.Linit_test:
    ldr r3, [r5, #4]
    cmp r8, r3
    blt .Linit_loop
    mvn r2, #0
    str r2, [r5]
    ldr r2, [r5, #0x10]
    add r1, r2, r1
    add r1, r1, #0x1F
    bic r1, r1, #0x1F
    str r1, [r5, #8]
    bic r1, r4, #0x1F
    str r1, [r5, #0x0C]
    bl OS_RestoreInterrupts
    ldr r0, [r5, #8]
    pop {r4, r5, r6, r7, r8, lr}
    bx lr
.size OS_InitAlloc, . - OS_InitAlloc
.L_heap_info_init:
    .word OSiHeapInfo

.global OS_SetCurrentHeap
.type OS_SetCurrentHeap, %function
OS_SetCurrentHeap:
    push {r4, r5, lr}
    sub sp, sp, #4
    mov r4, r0
    mov r5, r1
    bl OS_DisableInterrupts
    ldr r1, .L_heap_info_current
    ldr r1, [r1, r4, lsl #2]
    ldr r4, [r1]
    str r5, [r1]
    bl OS_RestoreInterrupts
    mov r0, r4
    add sp, sp, #4
    pop {r4, r5, lr}
    bx lr
.size OS_SetCurrentHeap, . - OS_SetCurrentHeap
.L_heap_info_current:
    .word OSiHeapInfo

.global OS_FreeToHeap
.type OS_FreeToHeap, %function
OS_FreeToHeap:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r7, r0
    mov r4, r1
    mov r6, r2
    bl OS_DisableInterrupts
    mov r5, r0
    ldr r0, .L_heap_info_free
    ldr r0, [r0, r7, lsl #2]
    cmp r4, #0
    ldrlt r4, [r0]
    sub r6, r6, #0x20
    ldr r1, [r0, #0x10]
    mov r0, #0x0C
    mla r7, r4, r0, r1
    ldr r0, [r7, #8]
    mov r1, r6
    bl DLExtract
    str r0, [r7, #8]
    ldr r0, [r7, #4]
    mov r1, r6
    bl DLInsert
    str r0, [r7, #4]
    mov r0, r5
    bl OS_RestoreInterrupts
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size OS_FreeToHeap, . - OS_FreeToHeap
.L_heap_info_free:
    .word OSiHeapInfo

.global OS_AllocFromHeap
.type OS_AllocFromHeap, %function
OS_AllocFromHeap:
    push {r4, r5, r6, r7, lr}
    sub sp, sp, #4
    mov r6, r0
    mov r5, r1
    mov r7, r2
    bl OS_DisableInterrupts
    mov r4, r0
    ldr r1, .L_heap_info_alloc
    ldr r1, [r1, r6, lsl #2]
    cmp r1, #0
    bne .Lalloc_have_arena
    bl OS_RestoreInterrupts
    mov r0, #0
    b .Lalloc_return
.Lalloc_have_arena:
    cmp r5, #0
    ldrlt r5, [r1]
    ldr r1, [r1, #0x10]
    mov r0, #0x0C
    mla r6, r5, r0, r1
    add r0, r7, #0x20
    add r0, r0, #0x1F
    bic r7, r0, #0x1F
    ldr r0, [r6, #4]
    mov r5, r0
    b .Lalloc_find_test
.Lalloc_find_loop:
    ldr r1, [r5, #8]
    cmp r7, r1
    ble .Lalloc_found
    ldr r5, [r5, #4]
.Lalloc_find_test:
    cmp r5, #0
    bne .Lalloc_find_loop
.Lalloc_found:
    cmp r5, #0
    bne .Lalloc_split_test
    mov r0, r4
    bl OS_RestoreInterrupts
    mov r0, #0
    b .Lalloc_return
.Lalloc_split_test:
    ldr r1, [r5, #8]
    sub r1, r1, r7
    cmp r1, #0x40
    bhs .Lalloc_split
    mov r1, r5
    bl DLExtract
    str r0, [r6, #4]
    b .Lalloc_link_used
.Lalloc_split:
    str r7, [r5, #8]
    add r2, r5, r7
    str r1, [r2, #8]
    ldr r0, [r5]
    str r0, [r5, r7]
    ldr r0, [r5, #4]
    str r0, [r2, #4]
    ldr r0, [r2, #4]
    cmp r0, #0
    strne r2, [r0]
    ldr r0, [r2]
    cmp r0, #0
    strne r2, [r0, #4]
    streq r2, [r6, #4]
.Lalloc_link_used:
    ldr r0, [r6, #8]
    mov r1, r5
    bl DLAddFront
    str r0, [r6, #8]
    mov r0, r4
    bl OS_RestoreInterrupts
    add r0, r5, #0x20
.Lalloc_return:
    add sp, sp, #4
    pop {r4, r5, r6, r7, lr}
    bx lr
.size OS_AllocFromHeap, . - OS_AllocFromHeap
.L_heap_info_alloc:
    .word OSiHeapInfo

.global DLInsert
.type DLInsert, %function
DLInsert:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r12, r0
    mov lr, #0
    b .Linsert_find_test
.Linsert_find_loop:
    cmp r1, r12
    bls .Linsert_found
    mov lr, r12
    ldr r12, [r12, #4]
.Linsert_find_test:
    cmp r12, #0
    bne .Linsert_find_loop
.Linsert_found:
    str r12, [r1, #4]
    str lr, [r1]
    cmp r12, #0
    beq .Linsert_previous
    str r1, [r12]
    ldr r3, [r1, #8]
    add r2, r1, r3
    cmp r2, r12
    bne .Linsert_previous
    ldr r2, [r12, #8]
    add r2, r3, r2
    str r2, [r1, #8]
    ldr r12, [r12, #4]
    str r12, [r1, #4]
    cmp r12, #0
    strne r1, [r12]
.Linsert_previous:
    cmp lr, #0
    beq .Linsert_new_head
    str r1, [lr, #4]
    ldr r2, [lr, #8]
    add r3, lr, r2
    cmp r3, r1
    bne .Linsert_return
    ldr r1, [r1, #8]
    add r1, r2, r1
    str r1, [lr, #8]
    str r12, [lr, #4]
    cmp r12, #0
    strne lr, [r12]
    b .Linsert_return
.Linsert_new_head:
    mov r0, r1
.Linsert_return:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size DLInsert, . - DLInsert

.global DLExtract
.type DLExtract, %function
DLExtract:
    ldr r3, [r1, #4]
    cmp r3, #0
    ldrne r2, [r1]
    strne r2, [r3]
    ldr r2, [r1]
    cmp r2, #0
    ldreq r0, [r1, #4]
    ldrne r1, [r1, #4]
    strne r1, [r2, #4]
    bx lr
.size DLExtract, . - DLExtract

.global DLAddFront
.type DLAddFront, %function
DLAddFront:
    str r0, [r1, #4]
    mov r2, #0
    str r2, [r1]
    cmp r0, #0
    strne r1, [r0]
    mov r0, r1
    bx lr
.size DLAddFront, . - DLAddFront
