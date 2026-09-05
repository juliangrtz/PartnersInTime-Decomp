#ifndef PARTNERS_IN_TIME_OVERLAY018_ATTACK_STATE_H
#define PARTNERS_IN_TIME_OVERLAY018_ATTACK_STATE_H

#include <game/battle_scene.h>
#include <nitro.h>

typedef struct Overlay18AttackObjectSource {
    BattleSceneObject *object;
    u8 unknown_04[0x7A];
    u16 mode;
} Overlay18AttackObjectSource;

typedef struct Overlay18AttackObjectController {
    Overlay18AttackObjectSource *source;
    BattleSceneObject *object;
    BattleSceneObject *variant_object;
    s32 resource_index;
    s32 counter;
    union {
        u32 flags;
        struct {
            u32 mode : 4;
            u32 variant : 2;
            u32 unknown_06_10 : 5;
            u32 unknown_11_31 : 21;
        } flag_bits;
    };
} Overlay18AttackObjectController;

typedef char Overlay18AttackObjectSource_SizeCheck[
    sizeof(Overlay18AttackObjectSource) == 0x80 ? 1 : -1];
typedef char Overlay18AttackObjectController_SizeCheck[
    sizeof(Overlay18AttackObjectController) == 0x18 ? 1 : -1];

#endif
