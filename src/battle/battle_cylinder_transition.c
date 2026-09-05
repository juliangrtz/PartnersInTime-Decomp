#include <game/battle_context.h>
#include <game/battle_cylinder_transition.h>
#include <game/save_data.h>

enum BattleCylinderTransitionOffset {
    BATTLE_CYLINDER_INTENSITY_OFFSET = 0x22,
    BATTLE_CYLINDER_SAVE_FLAGS_OFFSET = 0x560,
    BATTLE_CYLINDER_VIEW_X_OFFSET = 0x6760,
    BATTLE_CYLINDER_VIEW_Y_OFFSET = 0x6762,
    BATTLE_CYLINDER_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleCylinderTransitionConstant {
    BATTLE_CYLINDER_ALTERNATE_DIRECTION_BIT = 3,
    BATTLE_CYLINDER_ACTIVE = 1 << 6,
    BATTLE_CYLINDER_VCOUNT_IRQ = 1 << 2,
    BATTLE_CYLINDER_DISPSTAT_VCOUNT_IRQ = 1 << 5,
    BATTLE_CYLINDER_SWAP_DISPLAYS = 1 << 15
};

typedef struct BattleCylinderRestoreTask {
    u32 unknown_00;
    void *allocation;
} BattleCylinderRestoreTask;

extern u32 SDK_AUTOLOAD_DTCM_START[];

#define BATTLE_DTCM_BASE ((u32)SDK_AUTOLOAD_DTCM_START)
#define BATTLE_DTCM_SYSTEM_RESERVED (BATTLE_DTCM_BASE + 0x3FC0)
#define BATTLE_INTERRUPT_CHECK_BUFFER (BATTLE_DTCM_SYSTEM_RESERVED + 0x38)

extern void func_02035284(void *allocation);
extern u32 OS_DisableIrqMask(u32 mask);
extern u32 OS_EnableIrqMask(u32 mask);
extern void OS_SetIrqFunction(u32 mask, void (*callback)(void));
extern void *func_ov002_02072508(
    void (*callback)(BattleCylinderRestoreTask *task), int argument,
    int unknown_2, int unknown_3);
extern void BattleCylinderTransition_Draw(int progress, int angle);

void BattleCylinderTransition_UpdateStandardTask(
    BattleCylinderTransitionTask *task);
void BattleCylinderTransition_UpdateAlternateFadeTask(
    BattleCylinderTransitionTask *task);
void BattleCylinderTransition_OnVCountInterrupt(void);
void BattleCylinderTransition_RestoreDisplayTask(
    BattleCylinderRestoreTask *task);
void BattleCylinderTransition_UpdateAlternateRotationTask(
    BattleCylinderTransitionTask *task);
void BattleCylinderTransition_UpdateAlternateRevealTask(
    BattleCylinderTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
void BattleCylinderTransition_InitializeTask(
    BattleCylinderTransitionTask *task) {
    BattleCylinderTransitionState *state = &task->state;

    state->progress = 0;
    state->angle = 0x600;
    if (((u32)*(u16 *)(gSaveData + BATTLE_CYLINDER_SAVE_FLAGS_OFFSET)
         << (31 - BATTLE_CYLINDER_ALTERNATE_DIRECTION_BIT)) >> 31 ==
        0) {
        task->callback = BattleCylinderTransition_UpdateStandardTask;
        return;
    }

    *(vu16 *)0x04000004 &= ~BATTLE_CYLINDER_DISPSTAT_VCOUNT_IRQ;
    OS_DisableIrqMask(BATTLE_CYLINDER_VCOUNT_IRQ);
    OS_SetIrqFunction(BATTLE_CYLINDER_VCOUNT_IRQ,
                      BattleCylinderTransition_OnVCountInterrupt);
    task->callback = BattleCylinderTransition_UpdateAlternateRevealTask;
}

void BattleCylinderTransition_UpdateAlternateRevealTask(
    BattleCylinderTransitionTask *task) {
    BattleCylinderTransitionState *state = &task->state;

    BattleCylinderTransition_Draw(state->progress, state->angle << 8);
    ++state->progress;
    if (state->progress >= 32) {
        task->callback =
            BattleCylinderTransition_UpdateAlternateRotationTask;
    }
}

void BattleCylinderTransition_UpdateAlternateRotationTask(
    BattleCylinderTransitionTask *task) {
    BattleCylinderTransitionState *state = &task->state;
    int angle;
    int split_line;

    state->angle -= 64;
    angle = state->angle;
    split_line = angle / 16 - 24;
    if (angle / 16 + 24 > 0) {
        BattleCylinderTransition_Draw(32, angle << 8);
        if (split_line < 0) {
            BattleCylinderTransition_Draw(32, (state->angle + 0xC00) << 8);
            func_ov002_02072508(
                BattleCylinderTransition_RestoreDisplayTask,
                split_line + 120, 0, 0);
        }
        return;
    }

    state->angle += 0xC00;
    BattleCylinderTransition_Draw(32, state->angle << 8);
    task->callback = BattleCylinderTransition_UpdateAlternateFadeTask;
}

void BattleCylinderTransition_RestoreDisplayTask(
    BattleCylinderRestoreTask *task) {
    func_02035284(task->allocation);
    OS_EnableIrqMask(BATTLE_CYLINDER_VCOUNT_IRQ);
    *(vu16 *)0x04000004 |= BATTLE_CYLINDER_DISPSTAT_VCOUNT_IRQ;
    *(vu16 *)0x04000304 &= ~BATTLE_CYLINDER_SWAP_DISPLAYS;
}

void BattleCylinderTransition_OnVCountInterrupt(void) {
    *(vu16 *)0x04000304 |= BATTLE_CYLINDER_SWAP_DISPLAYS;
    *(vu16 *)0x04000004 &= ~BATTLE_CYLINDER_DISPSTAT_VCOUNT_IRQ;
    OS_DisableIrqMask(BATTLE_CYLINDER_VCOUNT_IRQ);
    *(vu32 *)BATTLE_INTERRUPT_CHECK_BUFFER |= BATTLE_CYLINDER_VCOUNT_IRQ;
}

void BattleCylinderTransition_UpdateAlternateFadeTask(
    BattleCylinderTransitionTask *task) {
    BattleCylinderTransitionState *state = &task->state;
    int intensity;

    BattleCylinderTransition_Draw(32, state->angle << 8);
    state->angle -= 64;
    if (state->angle <= 1600) {
        intensity = 32 - (1600 - state->angle) / 32;
        if (intensity > 32) {
            intensity = 32;
        }
        if (intensity < 0) {
            intensity = 0;
        }
        *(u16 *)(gBattleContext + BATTLE_CYLINDER_INTENSITY_OFFSET) =
            intensity;
        if (intensity == 0) {
            *(u32 *)(gBattleContext +
                     BATTLE_CYLINDER_RUNTIME_FLAGS_OFFSET) &=
                ~BATTLE_CYLINDER_ACTIVE;
            task->callback = 0;
        }
    }
}

void BattleCylinderTransition_UpdateStandardTask(
    BattleCylinderTransitionTask *task) {
    BattleCylinderTransitionState *state = &task->state;
    int intensity;

    BattleCylinderTransition_Draw(state->progress, state->angle << 8);
    *(u16 *)(gBattleContext + BATTLE_CYLINDER_VIEW_X_OFFSET) = 0;
    *(u16 *)(gBattleContext + BATTLE_CYLINDER_VIEW_Y_OFFSET) = 0;
    ++state->progress;
    if (state->progress >= 32) {
        intensity = 64 - state->progress;
        if (intensity > 32) {
            intensity = 32;
        }
        if (intensity < 0) {
            intensity = 0;
        }
        *(u16 *)(gBattleContext + BATTLE_CYLINDER_INTENSITY_OFFSET) =
            intensity;
        if (intensity == 0) {
            *(u32 *)(gBattleContext +
                     BATTLE_CYLINDER_RUNTIME_FLAGS_OFFSET) &=
                ~BATTLE_CYLINDER_ACTIVE;
            task->callback = 0;
        }
    }
}
