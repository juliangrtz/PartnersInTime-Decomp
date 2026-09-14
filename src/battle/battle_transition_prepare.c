#include <game/battle_transition.h>
#include <nitro.h>
#include <game/battle_context.h>
#include <game/battle_background.h>
#include <game/save_data.h>

#define REG32(address) (*(volatile u32 *)(address))
extern void (*data_ov002_020c03b4[])(BattleAITask *);
extern void *BattleTransfer_EnqueueAfterMapping(void (*)(void *), void *, int, int);
void BattleTransition_ShowBattleLayers(void *unused);
void BattleTransition_PrepareReturn(BattleAITask *task);

BattleAITask *BattleTransition_BeginReturn(BattleAITask *task) {
    if (!task)
        task = BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 35652), 0);
    task->callback = BattleTransition_PrepareReturn;
    return task;
}

void BattleTransition_PrepareReturn(BattleAITask *task) {
    /* Prime the pipeline with a black quad before selecting the return effect. */
    REG32(0x04000440) = 2;
    REG32(0x04000454) = 0;
    REG32(0x04000470) = 0x8000;
    REG32(0x04000470) = 0x6000;
    REG32(0x04000470) = 0;
    REG32(0x0400046c) = 0x4000;
    REG32(0x0400046c) = 0x4000;
    REG32(0x0400046c) = 0x1000;
    REG32(0x040004a4) = 0x001f00c0;
    REG32(0x040004a8) = 0;
    REG32(0x04000480) = 0;
    REG32(0x04000500) = 1;
    REG32(0x0400048c) = 0xe800e000;
    REG32(0x0400048c) = 100;
    REG32(0x0400048c) = 0xe8002000;
    REG32(0x0400048c) = 100;
    REG32(0x0400048c) = 0x18002000;
    REG32(0x0400048c) = 100;
    REG32(0x0400048c) = 0x1800e000;
    REG32(0x0400048c) = 100;
    REG32(0x04000504) = 0;
    if (*(u32 *)(gBattleContext + 24) != 0x1002)
        return;
    BattleBackground_RequestToggle();
    BattleTransfer_EnqueueAfterMapping(BattleTransition_ShowBattleLayers, 0, 0, 0);
    data_ov002_020c03b4[((u32) * (u16 *)(gSaveData + 1376) << 19) >> 28](task);
}

void BattleTransition_ShowBattleLayers(void *unused) {
    *(volatile u16 *)0x04000304 &= ~0x8000;
    REG32(0x04000000) = (REG32(0x04000000) & 0xffffe0ff) | 0xf00;
}
