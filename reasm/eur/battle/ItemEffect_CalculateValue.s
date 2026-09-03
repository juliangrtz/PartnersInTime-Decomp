/* Calculate the numeric result encoded by one 20-byte item record. */

.syntax unified
.arch armv5te
.arm
.section .pit_ItemEffect_CalculateValue, "ax", %progbits
.balign 4

.equ ITEM_TAG, 0x2000
.equ ITEM_INDEX_MASK, 0x0FFF
.equ ITEM_RECORD_SIZE, 0x14
.equ ITEM_RECORD_EFFECT_FLAGS, 0x0C
.equ ITEM_RECORD_MAGNITUDE, 0x10

.extern gItemRecords

.global ItemEffect_CalculateValue
.type ItemEffect_CalculateValue, %function
ItemEffect_CalculateValue:
    and r3, r0, #ITEM_TAG
    cmp r3, #ITEM_TAG
    movne ip, #0
    ldreq r3, .L_item_index_mask
    ldreq ip, .L_item_records
    andeq r3, r0, r3
    moveq r0, #ITEM_RECORD_SIZE
    mlaeq ip, r3, r0, ip

    ldr r0, [ip, #ITEM_RECORD_EFFECT_FLAGS]
    mov r0, r0, lsl #1
    mov r0, r0, lsr #25
    cmp r0, #7
    addls pc, pc, r0, lsl #2
    b .L_default_zero
    b .L_add_fixed
    b .L_add_fixed_second_kind
    b .L_percent_of_max
    b .L_effect_zero
    b .L_add_percent_of_max
    b .L_add_percent_of_max
    b .L_add_percent_of_max
    b .L_effect_seven_zero

.L_add_fixed:
    ldrh r0, [ip, #ITEM_RECORD_MAGNITUDE]
    add r0, r1, r0
    cmp r2, r0
    movge r2, r0
    mov r0, r2
    bx lr

.L_add_fixed_second_kind:
    ldrh r0, [ip, #ITEM_RECORD_MAGNITUDE]
    add r0, r1, r0
    cmp r2, r0
    movge r2, r0
    mov r0, r2
    bx lr

.L_percent_of_max:
    ldrh r0, [ip, #ITEM_RECORD_MAGNITUDE]
    ldr r1, .L_divide_by_100_magic
    mul r0, r2, r0
    add r3, r0, #50
    smull r0, ip, r1, r3
    mov ip, ip, asr #5
    mov r0, r3, lsr #31
    add ip, r0, ip
    cmp r2, ip
    movge r2, ip
    mov r0, r2
    bx lr

.L_effect_zero:
    mov r0, #0
    bx lr

.L_add_percent_of_max:
    ldrh r0, [ip, #ITEM_RECORD_MAGNITUDE]
    ldr r1, .L_divide_by_100_magic
    mul r0, r2, r0
    add r3, r0, #50
    smull r0, ip, r1, r3
    mov ip, ip, asr #5
    mov r0, r3, lsr #31
    add ip, r0, ip
    add r0, r2, ip
    bx lr

.L_effect_seven_zero:
    mov r0, #0
    bx lr
.L_default_zero:
    mov r0, #0
    bx lr

.L_item_index_mask:
    .word ITEM_INDEX_MASK
.L_item_records:
    .word gItemRecords
.L_divide_by_100_magic:
    .word 0x51EB851F
.size ItemEffect_CalculateValue, . - ItemEffect_CalculateValue
