#include <game/battle_ai.h>
#include <game/battle_cylinder_wipe.h>
#include <game/save_data.h>

enum BattleCylinderWipeOffset {
    BATTLE_CYLINDER_WIPE_SAVE_FLAGS_OFFSET = 0x560
};

enum BattleCylinderWipeConstant {
    BATTLE_CYLINDER_WIPE_ALTERNATE_DIRECTION_BIT = 3,
    BATTLE_CYLINDER_WIPE_VCOUNT_IRQ = 1 << 2,
    BATTLE_CYLINDER_WIPE_DISPSTAT_VCOUNT_IRQ = 1 << 5,
    BATTLE_CYLINDER_WIPE_SWAP_DISPLAYS = 1 << 15
};

typedef struct BattleCylinderWipeRestoreTask {
    u32 unknown_00;
    void *allocation;
} BattleCylinderWipeRestoreTask;

extern u32 SDK_AUTOLOAD_DTCM_START[];

#define BATTLE_WIPE_DTCM_BASE ((u32)SDK_AUTOLOAD_DTCM_START)
#define BATTLE_WIPE_DTCM_SYSTEM_RESERVED (BATTLE_WIPE_DTCM_BASE + 0x3FC0)
#define BATTLE_WIPE_INTERRUPT_CHECK_BUFFER \
    (BATTLE_WIPE_DTCM_SYSTEM_RESERVED + 0x38)

extern void func_02035284(void *allocation);
extern u32 OS_DisableIrqMask(u32 mask);
extern u32 OS_EnableIrqMask(u32 mask);
extern void OS_SetIrqFunction(u32 mask, void (*callback)(void));
extern void *func_ov002_02072508(
    void (*callback)(BattleCylinderWipeRestoreTask *task), int argument,
    int unknown_2, int unknown_3);
extern BattleAITask *func_ov002_020b5ef0(BattleAITask *task);
extern void BattleCylinderWipe_Draw(int progress, int angle, int captured);

void BattleCylinderWipe_UpdateFinishTask(BattleCylinderWipeTask *task);
void BattleCylinderWipe_OnVCountInterrupt(void);
void BattleCylinderWipe_RestoreDisplayTask(
    BattleCylinderWipeRestoreTask *task);
void BattleCylinderWipe_UpdateRotationTask(BattleCylinderWipeTask *task);
void BattleCylinderWipe_UpdateRevealTask(BattleCylinderWipeTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleCylinderWipeTask *BattleCylinderWipe_InitializeTask(
    BattleCylinderWipeTask *task) {
    BattleCylinderWipeState *state;

    task->state.progress = 0;
    state = &task->state;
    state->angle = 0x600;
    state->velocity = 0;
    state->flags.bits.captured = 0;
    task->callback = BattleCylinderWipe_UpdateRevealTask;
    return task;
}

void BattleCylinderWipe_UpdateRevealTask(BattleCylinderWipeTask *task) {
    BattleCylinderWipeState *state = &task->state;

    BattleCylinderWipe_Draw(state->progress, state->angle << 8, 0);
    ++state->progress;
    if (state->progress >= 32) {
        state->velocity = -112;
        if (((u32)*(u16 *)(gSaveData + BATTLE_CYLINDER_WIPE_SAVE_FLAGS_OFFSET)
             << (31 - BATTLE_CYLINDER_WIPE_ALTERNATE_DIRECTION_BIT)) >> 31 ==
            0) {
            task->callback = BattleCylinderWipe_UpdateFinishTask;
            return;
        }
        *(vu16 *)0x04000004 &= ~BATTLE_CYLINDER_WIPE_DISPSTAT_VCOUNT_IRQ;
        OS_DisableIrqMask(BATTLE_CYLINDER_WIPE_VCOUNT_IRQ);
        OS_SetIrqFunction(BATTLE_CYLINDER_WIPE_VCOUNT_IRQ,
                          BattleCylinderWipe_OnVCountInterrupt);
        task->callback = BattleCylinderWipe_UpdateRotationTask;
    }
}

void BattleCylinderWipe_UpdateRotationTask(BattleCylinderWipeTask *task) {
    BattleCylinderWipeState *state = &task->state;
    int angle;
    int split_line;

    state->angle += state->velocity;
    state->velocity += 7;
    angle = state->angle;
    split_line = angle / 16 + 24;
    if (angle / 16 - 24 < 192) {
        BattleCylinderWipe_Draw(32, angle << 8, 0);
        if (split_line > 192) {
            BattleCylinderWipe_Draw(32, (state->angle - 0xC00) << 8, 0);
            func_ov002_02072508(BattleCylinderWipe_RestoreDisplayTask,
                                split_line - 120, 0, 0);
        }
        return;
    }

    func_ov002_02072508(BattleCylinderWipe_RestoreDisplayTask, -1, 0, 0);
    state->angle -= 0xC00;
    BattleCylinderWipe_Draw(32, state->angle << 8, 0);
    task->callback = BattleCylinderWipe_UpdateFinishTask;
}

void BattleCylinderWipe_RestoreDisplayTask(
    BattleCylinderWipeRestoreTask *task) {
    if ((int)task->allocation != -1) {
        func_02035284(task->allocation);
        OS_EnableIrqMask(BATTLE_CYLINDER_WIPE_VCOUNT_IRQ);
        *(vu16 *)0x04000004 |= BATTLE_CYLINDER_WIPE_DISPSTAT_VCOUNT_IRQ;
    }
    *(vu16 *)0x04000304 &= ~BATTLE_CYLINDER_WIPE_SWAP_DISPLAYS;
}

void BattleCylinderWipe_OnVCountInterrupt(void) {
    *(vu16 *)0x04000304 |= BATTLE_CYLINDER_WIPE_SWAP_DISPLAYS;
    *(vu16 *)0x04000004 &= ~BATTLE_CYLINDER_WIPE_DISPSTAT_VCOUNT_IRQ;
    OS_DisableIrqMask(BATTLE_CYLINDER_WIPE_VCOUNT_IRQ);
    *(vu32 *)BATTLE_WIPE_INTERRUPT_CHECK_BUFFER |=
        BATTLE_CYLINDER_WIPE_VCOUNT_IRQ;
}

void BattleCylinderWipe_UpdateFinishTask(BattleCylinderWipeTask *task) {
    BattleCylinderWipeState *state = &task->state;
    int previous_angle;

    BattleCylinderWipe_Draw(32, state->angle << 8,
                            state->flags.bits.captured);
    previous_angle = state->angle;
    state->angle = previous_angle + state->velocity;
    state->velocity += 7;
    if (previous_angle < 1600 && state->angle >= 1600) {
        func_ov002_020b5ef0(0);
        state->flags.bits.captured = 1;
    }
    if (state->angle >= 3456) {
        task->callback = 0;
    }
}
