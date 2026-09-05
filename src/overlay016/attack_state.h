#ifndef PARTNERS_IN_TIME_OVERLAY016_ATTACK_STATE_H
#define PARTNERS_IN_TIME_OVERLAY016_ATTACK_STATE_H

#include <game/battle_scene.h>
#include <nitro.h>

enum Overlay16AttackConstant {
    OVERLAY16_MOTION_TABLE_COUNT = 20,
    OVERLAY16_MOTION_TABLE_STEP = 16,
    OVERLAY16_MOTION_RECORD_SIZE = 20,
    OVERLAY16_MOTION_RECORD_HEADER_SIZE = 8,
    OVERLAY16_MOTION_RECORD_TABLE_OFFSET = 0xD3B0,
};

typedef struct Overlay16MotionPoint {
    s32 x;
    s32 y;
} Overlay16MotionPoint;

typedef struct Overlay16AttackObjectState {
    BattleSceneObject *object;
    void *effect;
    union {
        u32 flags;
        struct {
            u8 state : 4;
            u8 variant : 2;
            u8 unknown_06_07 : 2;
            u8 unknown_08_31[3];
        } flag_bits;
    };
} Overlay16AttackObjectState;

typedef char Overlay16MotionPoint_SizeCheck[
    sizeof(Overlay16MotionPoint) == 8 ? 1 : -1];
typedef char Overlay16AttackObjectState_SizeCheck[
    sizeof(Overlay16AttackObjectState) == 0xC ? 1 : -1];

extern Overlay16MotionPoint gOverlay16MotionTable[
    OVERLAY16_MOTION_TABLE_COUNT];
extern Overlay16MotionPoint gOverlay16MotionDeltas[
    OVERLAY16_MOTION_TABLE_COUNT];
extern int gOverlay16MotionTableOffset;

#endif
