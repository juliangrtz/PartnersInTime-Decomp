#ifndef PIT_OVERLAY026_ITEM_INTERNAL_H
#define PIT_OVERLAY026_ITEM_INTERNAL_H
#include <game/battle_scene.h>
extern "C" {
#include <game/battle_actor.h>
#include <game/battle_object.h>
#include <game/battle_effect.h>
#include <game/heap.h>
#include <game/save_data.h>
#include <game/overlay026_attack.h>
#include "attack_state.h"
}
struct Overlay26PartyEffectSlot {
    BattleSceneObject *object;
    u8 unknown_04[16];
    BattleEffect *effect;
    u8 unknown_18[4];
};
struct Overlay26ItemUser {
    union {
        BattlePartyActor party;
        struct {
            u8 unknown_00[120];
            void (*callback)(Overlay26ItemUser *);
        };
    };
    u16 unknown_84[2], item;
};
typedef char Overlay26PartyEffectSlotSize[sizeof(Overlay26PartyEffectSlot) == 28 ? 1 : -1];
typedef char Overlay26ItemUserSize[sizeof(Overlay26ItemUser) == 140 ? 1 : -1];
extern "C" {
extern u8 *gBattleContext;
extern s8 data_ov026_020c6520[][6][3];
extern s8 data_ov026_020c64e0[][6];
extern s8 data_ov026_020c63d8[][3], data_ov026_020c6420[][3], data_ov026_020c6468[][3],
    data_ov026_020c6498[][3];
void func_0202cbd4(void *, int, int);
void func_ov026_020c2d38(Overlay26ItemUser *);
void func_ov026_020c3054(Overlay26ItemUser *);

void Overlay26Attack_LoadResource(u16 resource, int size, int resource_index, int resource_id,
                                  int compressed);
void Overlay26Attack_LaunchItem(Overlay26ItemUser *user, int actor_id, int item_id, int velocity);
void Overlay26Attack_SpawnPartyEffect(int actor_id);
Overlay26PartyEffectSlot *Overlay26Attack_GetPartyEffectSlot(int actor_id);
int Overlay26Attack_GetPhasePosition(s16 *position, int phase, BattlePartyActor *actor, int relative);
void Overlay26Attack_GetActorPosition(s16 *position, BattlePartyActor *actor, int relative);
void Overlay26Attack_GetViewPosition(const s8 (*offsets)[3], s16 *position, int formation,
                                     BattleSceneObject *object, int relative);
void Overlay26Attack_GetWorldPosition(const s8 (*offsets)[3], s16 *position, int formation,
                                      BattleSceneObject *object, int relative);
void Overlay26Attack_SetFacingAnimation(BattlePartyActor *actor);
void Overlay26Attack_Initialize(Overlay26ItemUser *user);
}
#endif
