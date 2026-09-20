#ifndef PIT_POCKET_CHOMP_INTERNAL_H
#define PIT_POCKET_CHOMP_INTERNAL_H

/*
 * Declarations shared inside the Pocket Chomp overlay.
 */
#include <game/pocket_chomp.h>
#include <game/battle_object_link.h>
#include <game/overlay018_attack.h>
#include "attack_state.h"
extern "C" {
#include <nitro/fx.h>
#include <game/battle_effect.h>
#include <game/overlay010_motion.h>
struct PocketChompAttackWorkPrefix {
    u8 unknown00[16];
    struct {
        u8 unknown_00 : 1, stop : 1, unknown2 : 6;
    } flags16;
    u8 unknown17[241];
    u16 target_id;
    u8 unknown260[372];
    PocketChompAdultMotion *active_adult;
    u8 unknown636[8];
    u16 blocked_target;
    u16 horizontal_offset, contact_offset, progress;
};
typedef char PocketChompAttackWorkPrefix_SizeCheck[sizeof(PocketChompAttackWorkPrefix) == 652 ? 1 : -1];
/* Complete 656-byte attack allocation. The common prefix is mostly opaque here. */
struct PocketChompAttackWork {
    u8 unknown00[16];
    struct { u8 unknown0 : 1, stop : 1, unknown2 : 6; } action_flags;
    s8 phase;
    u8 unknown18[240];
    u16 target_id;
    u8 unknown260[44];
    PocketChompAdultMotion adults[2];
    PocketChompSupport supports[2];
    PocketChomp center;
    PocketChompAdultMotion *active_adult;
    PocketChompSupport *active_support, *other_support;
    u16 blocked_target, horizontal_offset, contact_offset, progress;
    struct { u16 direction : 1, unknown : 15; } flags;
    u16 padding;
};
typedef char PocketChompAttackWorkSize[sizeof(PocketChompAttackWork) == 656 ? 1 : -1];

extern PocketChompAttackWorkPrefix *data_ov002_020c0710;
extern s16 FX_SinCosTable_[];
extern int data_ov018_020c7180[];
extern int Random_NextModulo(int), _s32_div_f(int, int);
extern void BattleMotion_StartFollower(BattleSceneObject *, BattleSceneObject *, int);
extern void func_0200940c(BattleModel *, s16);
extern void func_ov018_020c2e50(void *, int);
extern void Overlay10Party_AverageStats(void *, int);
extern int func_ov018_020c2c14(int, BattleSceneObject *);
extern void func_ov018_020c4c70(PocketChompSupport *, PocketChomp *);
extern void func_ov018_020c3ea8(PocketChomp *);
extern void func_ov018_020c40b8(PocketChomp *, int, int, int);
extern int func_ov002_02072004(BattleSceneObject *, int, int, int, int);
}
#endif
