/* Smash Eggs allocation and entry setup, overlay 15, 0x020C5B4C-0x020C5E4C. */
#include "actor_internal.h"
#include "../attack_common_ov010/attack_work.h"
#include <game/overlay010_attack.h>
extern "C" {
void BattleFeedback_LoadResource();
int Overlay10Party_HasBadgeTwo();
extern int data_ov015_020c5e70[], data_ov015_020c5e78[];
extern u32 data_ov015_020c5ec0[];
extern s16 data_ov015_020c5e90[][3];
void Overlay15Attack_Initialize(BattlePartyActor *user)
{
    Overlay15AttackContext *context = (Overlay15AttackContext *)
        Overlay10Attack_CreateWork((Overlay10ActionActor *)user, sizeof(Overlay15AttackContext), 0);
    BattlePartyActor *owners[2];
    owners[0] = user;
    owners[1] = (BattlePartyActor *)BattleActor_GetById(user->linked_object_id);
    int participant_index;
    Overlay15AttackModelController *controller = context->actors;
    Overlay15AttackHitMotion *motion = &context->actors[0].hit_motion;
    /* Both pointers advance by one controller, including its embedded motion. */
    for (participant_index = 0; participant_index < 2;
         ++participant_index, ++controller,
         motion = (Overlay15AttackHitMotion *)((u8 *)motion + sizeof(*controller))) {
        BattlePartyActor *owner = owners[participant_index];
        Overlay15Attack_InitializeActorController(controller,
            (Overlay15AttackModelOwner *)owner, (u16)(participant_index + 53),
            participant_index);
        if (owner->formation_index >= 4) {
            Overlay15Attack_InitializeHitMotion(motion,
                BattleSceneObject_GetById((u16)(participant_index + 45)),
                data_ov015_020c5e70[participant_index]);
        }
    }
    Overlay15Attack_InitializeObjectPair(&context->pair);
    context->power = data_ov015_020c5e78[0];
    BattleObjectData_AllocateLoadBuffer(52, 8192);
    BattleObjectData_AllocateLoadBuffer(55, 8192);
    BattleObjectData_AllocateLoadBuffer(53, 36864);
    BattleObjectData_AllocateLoadBuffer(54, 36864);
    BattleObjectData_AllocateLoadBuffer(56, 4096);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_ConfigureLoad(55, 2464, 1);
    BattleObjectData_ConfigureLoad(53,
        owners[0]->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattlePartyActor *partner = owners[1];
    BattleObjectData_ConfigureLoad(54,
        partner->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_ConfigureLoad(56, 2432, 1);
    BattleObjectData_EnsureLoaded(52, 0xc0000063);
    BattleObjectData_EnsureLoaded(55, 0xc2000006);
    BattleObjectData_EnsureLoaded(53, data_ov015_020c5ec0[owners[0]->formation_index]);
    BattleObjectData_EnsureLoaded(54, data_ov015_020c5ec0[partner->formation_index]);
    BattleObjectData_EnsureLoaded(56, 0xc100009d);
    BattleFeedback_LoadResource();
    *(s16 *)(gBattleContext + 50) = 6;
    *(s16 *)(gBattleContext + 48) = 12;
    if (Overlay10Party_HasBadgeTwo()) {
        Overlay15Attack_ApplyBadgeMotionAdjustments();
    }
    s16 center[2];
    Overlay10Enemy_GetCenter(center);
    Overlay10Attack_BeginEntry((Overlay10ActionActor *)user, Overlay15Attack_UpdateEntry);
    /* The allocation starts with the shared 304-byte attack work prefix. */
    Overlay10Work *common = (Overlay10Work *)context;
    for (int i = 0; i < 2; ++i) {
        const s16 *home = data_ov015_020c5e90[context->actors[i].primary->formation_index & 1];
        common->home[i][0] = center[0] + home[0];
        common->home[i][1] = home[1];
        common->duration[i] = home[2];
        common->sound[i] = 61;
    }
    common->background_x = center[0] - 128;
    common->background_y = 0;
    common->background_duration = 24;
    common->mirror = (u8)((user->formation_index & 1) ^ 1);
    common->prompt_x = (user->formation_index & 1) ? 32 : -16;
    common->prompt_y = 0;
}
}
