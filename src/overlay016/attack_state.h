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
    union {
        u32 motion_flags;
        struct {
            u32 motion_phase : 4;
            u32 unknown_motion_04_31 : 28;
        } motion_flag_bits;
    };
    union {
        u32 control_flags;
        u8 control_byte;
        struct {
            u8 state : 4;
            u8 variant : 2;
            u8 unknown_06_07 : 2;
            u8 unknown_09;
            u8 unknown_0a;
            u8 unknown_0b;
        } control_flag_bits;
    };
} Overlay16AttackObjectState;

typedef struct Overlay16AttackTargetReference {
    BattleSceneObject *object;
    u8 unknown_04[0x12];
    union {
        u8 flags;
        struct {
            u8 unknown_00_04 : 5;
            u8 offset_variant : 2;
            u8 unknown_07 : 1;
        } flag_bits;
    };
} Overlay16AttackTargetReference;

typedef struct Overlay16AttackTargetParameters {
    u8 unknown_00[2];
    s8 x;
    s8 z;
    u8 unknown_04;
} Overlay16AttackTargetParameters;

typedef struct Overlay16AttackTargetOffset {
    u8 unknown_00[5];
    Overlay16AttackTargetParameters parameters;
} Overlay16AttackTargetOffset;

typedef char Overlay16MotionPoint_SizeCheck[
    sizeof(Overlay16MotionPoint) == 8 ? 1 : -1];
typedef char Overlay16AttackObjectState_SizeCheck[
    sizeof(Overlay16AttackObjectState) == 0xC ? 1 : -1];

extern Overlay16MotionPoint gOverlay16MotionTable[
    OVERLAY16_MOTION_TABLE_COUNT];
extern Overlay16MotionPoint gOverlay16MotionDeltas[
    OVERLAY16_MOTION_TABLE_COUNT];
extern int gOverlay16MotionTableOffset;
extern Overlay16AttackTargetOffset gOverlay16AttackTargetOffsets[];

#endif
