/* Party VM slot 2 is serviced by its dedicated resident updater. */

.syntax unified
.arch armv5te
.arm
.section .pit_BattleAI_StartPartyVmSlot2, "ax", %progbits
.balign 4

.global BattleAI_StartPartyVmSlot2
.type BattleAI_StartPartyVmSlot2, %function
BattleAI_StartPartyVmSlot2:
    bx lr
.size BattleAI_StartPartyVmSlot2, . - BattleAI_StartPartyVmSlot2
