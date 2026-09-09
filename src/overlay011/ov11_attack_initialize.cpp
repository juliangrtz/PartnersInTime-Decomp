#include "shell_internal.h"

extern "C" {
void Overlay11Attack_Initialize(BattlePartyActor *user, int resource, int configuration)
{
    Overlay11AttackWork *work = func_ov010_020c2978(user, 612, data_ov011_020c5f20[configuration] == 2);
    work->parameter = data_ov011_020c5fd0[configuration][0];
    work->parameter2 = data_ov011_020c5fdc[configuration][0];
    work->resource = resource;
    work->variant = configuration;
    work->controller_config = &data_ov011_020c6000[configuration];
    work->particle_config = &data_ov011_020c5fac[configuration];
    if (Overlay10Party_HasBadgeTwo())
        func_ov011_020c2ae0();
    u16 partner_resource;
    BattlePartyActor *partner = (BattlePartyActor *)BattleActor_GetById(user->linked_object_id);
    u16 user_resource = user->formation_index >= 4 ? 57 : user->actor.scene_object->resource->object_data_id;
    partner_resource = partner->actor.scene_object->resource->object_data_id;
    Overlay11Controller_Initialize(&work->controllers[0], user->actor.scene_object->actor_id, user, 53,
                                   user_resource, data_ov011_020c6140[configuration], 0);
    Overlay11Controller_Initialize(&work->controllers[1], partner->actor.scene_object->actor_id, user, 54,
                                   partner_resource, data_ov011_020c6140[configuration], 1);
    Overlay11Attack_InitializeProjectile((Overlay11AttackProjectile *)&work->projectile, (int)user,
                                         configuration);
    u32 formation = partner->formation_index;
    int partner_component, partner_buffer_size;
    if (formation >= 4) {
        partner_component = formation & 1 ? 0xc000002e : 0xc000002d;
        partner_buffer_size = 28672;
    } else {
        partner_component = work->controllers[1].config->resource;
        partner_buffer_size = work->controllers[1].config->buffer_size;
    }
    BattleObjectData_AllocateLoadBuffer(52, 4096);
    BattleObjectData_AllocateLoadBuffer(53, work->controllers[0].config->buffer_size);
    BattleObjectData_AllocateLoadBuffer(54, partner_buffer_size);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_ConfigureLoad(
        53, work->controllers[0].actor->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_ConfigureLoad(
        54, work->controllers[1].actor->actor.scene_object->resource->flags.bits.resource_index - 1, 1);
    BattleObjectData_EnsureLoaded(52, resource);
    BattleObjectData_EnsureLoaded(53, work->controllers[0].config->resource);
    BattleObjectData_EnsureLoaded(54, partner_component);
    if (user->formation_index >= 4) {
        func_ov011_020c39fc(&work->support, user, configuration, 0);
        BattleObjectData_AllocateLoadBuffer(55, 4096);
        BattleObjectData_ConfigureLoad(55, 2496, 0);
        BattleObjectData_EnsureLoaded(55, data_ov011_020c5f64[user->formation_index & 1]);
        BattleObjectData_AllocateLoadBuffer(56, 30720);
        BattleObjectData_ConfigureLoad(56, BattleObjectData_GetLoadState(53)->flags.bits.resource_index - 1,
                                       1);
        BattleObjectData_EnsureLoaded(56, data_ov011_020c5f6c[user->formation_index & 1]);
        BattleObjectData_AllocateLoadBuffer(57, 102400);
        BattleObjectData_ConfigureLoad(57, BattleObjectData_GetLoadState(53)->flags.bits.resource_index - 1,
                                       1);
        BattleObjectData_EnsureLoaded(57, data_ov011_020c5f74[user->formation_index & 1]);
    }
    int effect = data_ov011_020c5f24[configuration];
    if (data_ov011_020c5f24[configuration]) {
        BattleObjectData_AllocateLoadBuffer(58, 8192);
        BattleObjectData_ConfigureLoad(58, 2660, 0);
        effect = (int)func_ov002_02091fd8(58, 0xc1000050);
    }
    BattleFeedback_LoadResource(effect);
    *(u16 *)(gBattleContext + 52096) = 0;
    Overlay10Attack_BeginEntry(user, func_ov011_020c57cc);
    int i;
    Overlay11AttackWork *row = work;
    i = 0;
    do {
        work->home[i].x = row->controllers[0].config->home_x;
        work->home[i].y = row->controllers[0].config->home_y;
        work->home_z[i] = row->controllers[0].config->home_z;
        ++i;
        row = (Overlay11AttackWork *)((u8 *)row + sizeof(Overlay11Controller));
    } while (i < 2);
}
}
