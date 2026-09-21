/* Mix Flowers allocation and resource setup, overlay 16, 0x020C6E24-0x020C7274. */
#include "pair_effects_internal.h"
#include "../attack_common_ov010/attack_work.h"
#include <game/overlay010_attack.h>
#include <game/battle_feedback.h>
extern "C" {
extern int data_ov016_020c72cc;
/* Two interior aliases into the side-dependent resource pairs, stride 8. */
extern u32 data_ov016_020c72f0[][2], data_ov016_020c72f4[][2];
int Overlay10Party_HasBadgeTwo();
void func_ov016_020c6ae8(BattlePartyActor *);
void BattleAttack_SetCallback(BattlePartyActor *, void (*)(BattlePartyActor *));
void Overlay16Attack_Initialize(BattlePartyActor *user)
{
    Overlay16AttackWork *work = (Overlay16AttackWork *)Overlay10Attack_CreateWork(
        (Overlay10ActionActor *)user, sizeof(Overlay16AttackWork), 1);
    int i;
    BattleSceneObject *object;
    int component;
    Overlay16Participant *participant = work->participants;
    Overlay16AttackObjectState *variant = work->variants;
    for (i = 0; i < 4; ++i) {
        switch (i) {
        case 0:
            object = BattleSceneObject_GetById(56);
            component = object->resource->flags.bits.resource_index - 1;
            break;
        case 1:
            object = BattleSceneObject_GetById(57);
            component = object->resource->flags.bits.resource_index - 1;
            break;
        case 2:
            object = BattleSceneObject_GetById(43);
            component = 2816;
            break;
        case 3:
            object = BattleSceneObject_GetById(44);
            component = 2864;
            break;
        }
        Overlay16Participant_Initialize(participant, object, i, component);
        Overlay16Attack_InitializeObjectState(variant, i);
        ++participant;
        ++variant;
    }
    /* Both scene objects in each effect are embedded in this allocation. */
    int effect_index;
    Overlay16PairEffect *effect = work->effects;
    for (effect_index = 0; effect_index < 32; ++effect_index, ++effect)
        Overlay16PairEffect_Initialize(effect, 53, 54);
    /* The attack begins with the common 304-byte work prefix. */
    ((Overlay10Work *)work)->parameter = data_ov016_020c72cc;
    BattlePartyActor *second, *first;
    if (user->formation_index & 1) {
        second = user;
        first = (BattlePartyActor *)BattleActor_GetById(user->linked_object_id);
    } else {
        first = user;
        second = (BattlePartyActor *)BattleActor_GetById(user->linked_object_id);
    }
    work->adults[0] = first;
    work->adults[1] = second;
    /* Slots 56/57 use the adult objects; 58/59 select resources by side. */
    BattleObjectData_AllocateLoadBuffer(56, 30720);
    BattleObjectData_AllocateLoadBuffer(57, 30720);
    BattleObjectData_AllocateLoadBuffer(58, 24576);
    BattleObjectData_AllocateLoadBuffer(59, 24576);
    BattleObjectData_AllocateLoadBuffer(60, 18432);
    BattleObjectData_AllocateLoadBuffer(61, 18432);
    BattleObjectData_AllocateLoadBuffer(52, 30720);
    BattleObjectData_AllocateLoadBuffer(53, 7168);
    BattleObjectData_AllocateLoadBuffer(54, 7168);
    BattleObjectData_AllocateLoadBuffer(55, 512);
    BattleObjectData_ConfigureLoad(56,
        first->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_ConfigureLoad(57,
        second->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_ConfigureLoad(58,
        first->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_ConfigureLoad(59,
        second->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_ConfigureLoad(60, 2816, 1);
    BattleObjectData_ConfigureLoad(61, 2864, 1);
    BattleObjectData_ConfigureLoad(52, 2688, 1);
    BattleObjectData_ConfigureLoad(53, 2432, 0);
    BattleObjectData_ConfigureLoad(55, 2912, 0);
    BattleObjectData_GetLoadState(54)->flags.bits.copy_flag = 0;
    int side = user->formation_index & 1;
    BattleObjectData_EnsureLoaded(56, 0xc0000043);
    BattleObjectData_EnsureLoaded(57, 0xc0000044);
    BattleObjectData_EnsureLoaded(58, data_ov016_020c72f0[side][0]);
    BattleObjectData_EnsureLoaded(59, data_ov016_020c72f4[side][0]);
    BattleObjectData_EnsureLoaded(60, 0xc000004c);
    BattleObjectData_EnsureLoaded(61, 0xc000004d);
    BattleObjectData_EnsureLoaded(52, 0xc0000051);
    BattleObjectData_EnsureLoaded(53, 0xc0000072);
    BattleObjectData_EnsureLoaded(54, 0xc0000072);
    BattleObjectData_EnsureLoaded(55, 0xc10000a5);
    BattleFeedback_LoadResource();
    if (Overlay10Party_HasBadgeTwo())
        Overlay16Attack_AdvanceMotionTable();
    s16 center[2];
    Overlay10Enemy_GetCenter(center);
    work->center_x = center[0];
    work->center_y = 140;
    work->entry_offset_x = work->center_x - 128;
    Overlay16PairController_Initialize(&work->controller, work->center_x, work->center_y);
    Overlay16LinkedObject_Initialize(&work->linked);
    BattleAttack_SetCallback(user, func_ov016_020c6ae8);
}
}
