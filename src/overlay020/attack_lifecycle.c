#include "attack_internal.h"

void Overlay20Attack_Create(BattlePartyActor *actor)
{
    Overlay20AttackWork *work;
    Overlay20AttackConfig *config = &data_ov020_020c4fd0[actor->formation_index];
    int component;
    data_ov002_020c0710 = GameHeap_Allocate(*(int *)(gBattleContext + 0xe154), 112, 0, 1);
    func_0202cbd4(data_ov002_020c0710, 0, 112);
    work = data_ov002_020c0710;
    *(u32 *)(gBattleContext + 0xd3a0) &= ~0x1000;
    Overlay20Attack_InitializeActor(&work->main, actor);
    component = actor->actor.scene_object->resource->flags.bits.resource_index - 1;
    BattleObjectData_AllocateLoadBuffer(52, 0x8000);
    BattleObjectData_ConfigureLoad(52, component, 1);
    BattleObjectData_EnsureLoaded(52, config->resources[0]);
    if (actor->formation_index >= 4) {
        Overlay20Attack_Initialize(&work->support, (BattlePartyActor *)BattleActor_GetById(
                                                       (u16)((actor->formation_index & 1) + 58)));
        BattleObjectData_AllocateLoadBuffer(53, 0x7800);
        BattleObjectData_AllocateLoadBuffer(54, 0x4000);
        BattleObjectData_AllocateLoadBuffer(55, 0x4000);
        BattleObjectData_ConfigureLoad(53, component, 1);
        BattleObjectData_ConfigureLoad(54, component, 1);
        BattleObjectData_ConfigureLoad(55, component, 1);
        BattleObjectData_EnsureLoaded(53, config->resources[1]);
        BattleObjectData_EnsureLoaded(54, config->resources[2]);
        BattleObjectData_EnsureLoaded(55, config->resources[3]);
    }
    BattleFeedback_LoadResource();
    *(u16 *)(gBattleContext + 0xcb80) = 0;
    func_ov002_020722ac(actor, Overlay20Attack_Update);
}

void Overlay20Attack_Update(BattlePartyActor *actor)
{
    Overlay20AttackWork *work = data_ov002_020c0710;
    *(u16 *)(gBattleContext + 0xcb80) = 0;
    func_ov020_020c326c(&work->support, &work->main);
    func_ov020_020c3eec(&work->main, &work->support);
    switch (work->phase) {
    case 0:
        if (!BattleFeedback_IsResourceLoadPending())
            work->phase = 1;
        break;
    case 1:
        Overlay20Attack_StartApproach(&work->main);
        work->phase = 2;
        break;
    case 2:
        if (!work->main.flags.bits.state && !work->support.flags.bits.state && BattleFeedback_IsEffectComplete())
            work->phase = 3;
        break;
    case 3:
        func_ov002_020722ac(actor, 0);
        data_ov002_020c0710 = 0;
        *(u16 *)(gBattleContext + 0xcb80) = 0;
        break;
    }
}

void Overlay20Attack_InitializeActor(Overlay20AttackState *state, BattlePartyActor *actor)
{
    state->primary_object = actor->actor.scene_object;
    state->secondary_object = BattleSceneObject_GetById(41);
    state->actor = actor;
    state->animation_argument = actor->actor.scene_object->resource->flags.bits.resource_index - 1;
    state->unknown_10 = 0;
    state->unknown_14 = 0;
    state->motion_duration = 0;
    state->flags.bits.state = 0;
    state->flags.raw = (state->flags.raw & ~0x1e0) | ((actor->formation_index & 15) << 5);
    state->primary_object->effect_anchor_z = -12;
}

void Overlay20Attack_ClearPhase(Overlay20AttackState *state)
{
    state->flags.bits.state = 0;
}
