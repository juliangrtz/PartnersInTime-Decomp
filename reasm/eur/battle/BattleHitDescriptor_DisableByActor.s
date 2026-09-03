/* Resolve and disable the hit descriptor owned by an actor/object ID. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleHitDescriptor_DisableByActor, "ax", %progbits
.balign 4

.extern BattleHitDescriptor_Disable
.extern BattleHitDescriptor_GetByActorId

.global BattleHitDescriptor_DisableByActor
.type BattleHitDescriptor_DisableByActor, %function
BattleHitDescriptor_DisableByActor:
    stmdb sp!, {lr}
    sub sp, sp, #4
    mov r0, r0, lsl #16
    mov r0, r0, lsr #16
    bl BattleHitDescriptor_GetByActorId
    bl BattleHitDescriptor_Disable
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
.size BattleHitDescriptor_DisableByActor, . - BattleHitDescriptor_DisableByActor
