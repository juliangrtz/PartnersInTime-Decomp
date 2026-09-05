#include <game/battle_ai.h>
#include <game/battle_scene.h>
#include <game/overlay016_attack.h>

#include "attack_state.h"

enum {
    OVERLAY16_OBJECT_MOTION_CHANNEL = 2,
    OVERLAY16_PACKED_INDEX_MASK = 0x00FFFFFF,
};

/* Metrowerks emits C functions in reverse source order. */
int Overlay16Attack_IsObjectMotionComplete(
    Overlay16AttackObjectState *state) {
    return BattleSceneObject_IsAnimationChannelActive(
               state->object, OVERLAY16_OBJECT_MOTION_CHANNEL) == 0;
}

int Overlay16Attack_IsObjectIdle(Overlay16AttackObjectState *state) {
    return state->flag_bits.state == 0;
}

void *Overlay16Attack_GetMotionRecord(u32 packed_index) {
    u8 *table = *(u8 **)(gBattleContext +
                         OVERLAY16_MOTION_RECORD_TABLE_OFFSET);

    return table + OVERLAY16_MOTION_RECORD_HEADER_SIZE +
           OVERLAY16_MOTION_RECORD_SIZE *
               (packed_index & OVERLAY16_PACKED_INDEX_MASK);
}

void Overlay16Attack_AdvanceMotionTable(void) {
    u32 index;

    for (index = 0; index < OVERLAY16_MOTION_TABLE_COUNT; ++index) {
        gOverlay16MotionTable[index].x +=
            gOverlay16MotionDeltas[index].x;
        gOverlay16MotionTable[index].y +=
            gOverlay16MotionDeltas[index].y;
    }
    gOverlay16MotionTableOffset += OVERLAY16_MOTION_TABLE_STEP;
}
