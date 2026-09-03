/* Initialize the ARM7 file-system state and its card back end once. */

.syntax unified
.arch armv4t
.arm
.section .pit_FS_Init, "ax", %progbits
.balign 4

.extern CARD_Init
.extern FSi_ArchiveList
.extern FSi_CardLockId
.extern FSi_DefaultDmaNo
.extern FSi_Initialized
.extern FSi_OverlayList
.extern OS_GetLockID

.global FS_Init
.type FS_Init, %function
FS_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_fs_initialized
    ldr r2, [r1]
    cmp r2, #0
    bne .L_fs_init_done
    mov r2, #1
    str r2, [r1]
    bl FSi_Init
.L_fs_init_done:
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_fs_initialized:
    .word FSi_Initialized
.size FS_Init, . - FS_Init

.global FSi_Init
.type FSi_Init, %function
FSi_Init:
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r1, .L_default_dma
    str r0, [r1]
    bl OS_GetLockID
    ldr r1, .L_card_lock_id
    str r0, [r1]
    mov r1, #0
    ldr r0, .L_archive_list
    str r1, [r0]
    str r1, [r0, #4]
    ldr r0, .L_overlay_list
    str r1, [r0]
    str r1, [r0, #4]
    bl CARD_Init
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.L_default_dma:
    .word FSi_DefaultDmaNo
.L_card_lock_id:
    .word FSi_CardLockId
.L_archive_list:
    .word FSi_ArchiveList
.L_overlay_list:
    .word FSi_OverlayList
.size FSi_Init, . - FSi_Init
