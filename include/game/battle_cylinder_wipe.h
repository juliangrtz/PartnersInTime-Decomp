#ifndef PIT_GAME_BATTLE_CYLINDER_WIPE_H
#define PIT_GAME_BATTLE_CYLINDER_WIPE_H

#include <nitro.h>

typedef union BattleCylinderWipeFlags {
    u16 raw;
    struct {
        u16 captured : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleCylinderWipeFlags;

typedef struct BattleCylinderWipeState {
    s16 progress;
    s16 angle;
    s16 velocity;
    BattleCylinderWipeFlags flags;
} BattleCylinderWipeState;

typedef struct BattleCylinderWipeTask {
    struct BattleCylinderWipeTask *next;
    void (*callback)(struct BattleCylinderWipeTask *task);
    struct BattleCylinderWipeTask **owner_slot;
    BattleCylinderWipeState state;
} BattleCylinderWipeTask;

typedef char BattleCylinderWipeState_SizeCheck[
    sizeof(BattleCylinderWipeState) == 8 ? 1 : -1];
typedef char BattleCylinderWipeTask_SizeCheck[
    sizeof(BattleCylinderWipeTask) == 0x14 ? 1 : -1];

BattleCylinderWipeTask *BattleCylinderWipe_InitializeTask(
    BattleCylinderWipeTask *task);

#endif
