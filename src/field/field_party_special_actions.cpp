#include <game/field_resources.h>
#include "field_party_internal.h"
#include <game/field_blink.h>
#include <game/field_auxiliary.h>
#include <game/field_geometry.h>
extern "C" {
extern const fx32 data_ov000_020c07f8[][2], data_ov000_020c07fc[][2];
}

#define RECORD(member) party->members[member]->state_record->resources
static inline void SaveBehavior(FieldPartyController *party, int member)
{
    if (!party->members[member]->presentation.behavior_saved) {
        party->members[member]->presentation.saved_behavior =
            (u16)party->members[member]->entity.saved_presentation_flag_bits.behavior_mode;
        party->members[member]->presentation.behavior_saved = 1;
    }
}
extern "C" void FieldParty_BeginState89(FieldPartyController *party)
{
    int member = 0;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->state_bits.unknown_01 = 0;
    do {
        SaveBehavior(party, member);
        party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 1;
        party->members[member]->entity.locomotion_state = 89;
        party->members[member]->bits.movement_mode = 8;
        FieldParty_BindActionResource(party, party->members[member], member + 51, 256, 0);
        if (!party->members[member]->entity.field_state_flag_bits.vertical_motion_active)
            FieldVertical_Start(&party->members[member]->entity, 15772, 1076, 0);
    } while (++member < 2);
}
extern "C" void FieldParty_LaunchToElevation(FieldPartyController *party, fx32 height)
{
    if (party->leader->bits.movement_mode == 8 && party->leader->entity.locomotion_state != 88 &&
        RECORD(0).flags.unknown_20_21 != 2 && party->follower->bits.movement_mode == 8 &&
        party->follower->entity.locomotion_state != 88 && RECORD(1).flags.unknown_20_21 != 2 &&
        (party->leader->entity.previous_support_entity_index == -1 ||
         party->leader->entity.position_z < height) &&
        (party->follower->entity.previous_support_entity_index == -1 ||
         party->follower->entity.position_z < height)) {
        if (party->areas[party->flags.field_screen]->room_id != 458 ||
            party->leader->entity.position_z - party->leader->entity.support_clearance <=
                party->follower->entity.position_z - party->follower->entity.support_clearance) {
            party->flags.previous_field_screen = party->flags.field_screen;
            for (int member = 0; member < 2; ++member) {
                if (RECORD(member).flags.unknown_20_21 == 1) {
                    RECORD(member).flags.unknown_20_21 = 0;
                    RECORD(member).flags.unknown_01_14 = 0;
                    FieldBlink_Stop(&party->members[member]->entity, 1);
                }
                party->members[member]->entity.field_state_flag_bits.track_ground = 0;
                FieldVertical_Stop(&party->members[member]->entity);
                FieldLinear3D_Start(&party->members[member]->entity, 1, 0, 0,
                                    height - party->members[member]->entity.position_z, 1024, 204, 16384,
                                    -614, 32, 32, 1, &party->members[member]->movement);
                party->members[member]->entity.locomotion_state = 91;
            }
            party->state.unknown_00 = 0;
            party->state.unknown_01 = 0;
        }
    }
}

#undef RECORD

extern "C" {
void func_ov000_020931b0(FieldPartyController *, FieldPartyEntity *, int);

void FieldParty_BeginBabyDrop(FieldPartyController *party)
{
    int member;
    party->flags.previous_field_screen = party->flags.field_screen;
    for (member = 0; member < 2; ++member) {
        party->members[member]->entity.field_state_flag_bits.track_ground = 1;
        party->members[member]->entity.base.cancel_linear_movement(&party->members[member]->movement, 0);
        FieldVertical_Start(&party->members[member]->entity, 0, 800, -8192);
        party->members[member]->entity.locomotion_state = 93;
    }
    party->flags.unknown_06 = 1;
}
void FieldParty_LandBabyDrop(FieldPartyController *party, int member)
{
    party->members[member]->entity.locomotion_state = 0;
    party->state_groups.members = (u16)(party->state_groups.members | (1 << member));
}
/* Room scripts use mode 5 for the baby-party drill/underground gimmick. */
void FieldParty_RestoreMode5(FieldPartyController *party, int preserve_state)
{
    FieldResourceContext *area;
    FieldPartyEntity *leader;
    const FieldPrimaryResource *resource;
    int index;
    if (!preserve_state) {
        party->flags.unknown_14 = 0;
        party->state_bits.unknown_01 = 0;
        FieldParty_RefreshActions(party);
        party->leader->entity.locomotion_state = 0;
        party->follower->entity.locomotion_state = 0;
        party->leader->bits.movement_mode = 5;
        party->follower->bits.movement_mode = 5;
        party->leader->entity.base.map_locomotion_state();
        FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
        if (party->flags.movement_active) {
            FieldEntity_SetLocomotionParameters(&party->follower->entity, 4096, 0, 4096, -4096, 0, 4096);
            FieldParty_InitializeFollowing(party, 65536);
        } else
            party->unknown_06c = 65536;
        party->flags.unknown_04 = 1;
        party->flags.unknown_06 = 1;
        party->state.unknown_00 = 0;
        party->state.unknown_01 = 1;
        party->leader->entity.unknown_3c8 = 0;
        party->leader->entity.unknown_3c8 |= 0x80;
        party->follower->entity.unknown_3c8 = 0;
        party->follower->entity.unknown_3c8 |= 0x80;
    }
    party->leader->entity.saved_presentation_flag_bits.has_saved_behavior = 0;
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 3;
    party->leader->entity.resource_index = party->leader->unknown_595;
    party->leader->entity.saved_presentation_flag_bits.has_saved_resource = 0;
    party->leader->entity.saved_presentation_flag_bits.has_saved_resource_animation = 0;
    party->leader->entity.animation_id = party->leader->entity.base_state_flag_bits.facing_direction;
    party->leader->entity.saved_presentation_flag_bits.has_saved_animation = 0;
    area = party->areas[party->flags.field_screen];
    index = area->special_resources[0];
    resource = index != -1 ? &area->primary[0][index] : 0;
    leader = party->leader;
    if (!leader->presentation.unknown_00) {
        leader->saved_animation_speed = leader->entity.animation_speed;
        party->leader->presentation.unknown_00 = 1;
    }
    FieldEntity_RebindRendererResources(&party->leader->entity,
        (const FieldPrimaryResource *)resource, 0, 0, -1, 1, 128);
    party->leader->presentation.unknown_05 = 1;
    party->leader->presentation.resource_index =
        party->areas[party->flags.field_screen]->special_resources[0];
    if (party->flags.movement_active)
        party->follower->entity.animation_speed = 128;
    FieldParty_SetMemberAction(party, 0, 30);
    FieldParty_SetMemberAction(party, 1, 4);
}
void FieldParty_BeginState76(FieldPartyController *party)
{
    FieldPartyEntity *follower, *auxiliary;
    FieldRenderObject *renderer;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_01 = 0;
    party->follower->entity.locomotion_state = 76;
    FieldParty_BindActionResource(party, party->follower, 24, 256, 1);
    party->follower->entity.render_object->state_flag_bits.behavior_state = 1;
    follower = party->follower;
    if (!follower->presentation.behavior_saved) {
        follower->presentation.saved_behavior =
            (u16)follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->follower->entity.field_state_flag_bits.unknown_16 = 1;
    auxiliary = party->follower->auxiliary;
    auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 1;
    auxiliary->entity.runtime_flag_bits.sync_horizontal = 0;
    auxiliary->entity.runtime_flag_bits.sync_vertical = 0;
    auxiliary->entity.runtime_flag_bits.unknown_02 = 0;
    auxiliary->entity.runtime_flag_bits.auto_priority_0 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_04 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_06 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_07 = 0;
    auxiliary->entity.field_state_flag_bits.body_collision_enabled = 1;
    auxiliary->auxiliary_bits.unknown_00 = 1;
    auxiliary->auxiliary_bits.unknown_01 = 1;
    auxiliary->auxiliary_motion.target = 0;
    auxiliary->auxiliary_bits.direction_mode = 0;
    auxiliary->auxiliary_bits.unknown_04 = 0;
    auxiliary->auxiliary_bits.unknown_05_06 = 0;
    auxiliary->auxiliary_bits.unknown_07_08 = 0;
    auxiliary->auxiliary_bits.unknown_09 = 1;
    auxiliary->entity.locomotion_state = party->leader->entity.locomotion_state;
    auxiliary->auxiliary_bits.unknown_19 = 0;
    auxiliary->auxiliary_bits.unknown_20_27 = 26;
    auxiliary->auxiliary_bits.unknown_28_31 = -1;
    auxiliary->auxiliary_motion.unknown_53c = 0;
    auxiliary->auxiliary_motion.unknown_540 = 0;
    auxiliary->auxiliary_motion.unknown_544 = 0;
    auxiliary->entity.animation_id = party->leader->entity.base_state_flag_bits.facing_direction;
    FieldEntity_ConfigureRendererResources(&auxiliary->entity,
        (const FieldPrimaryResource *)(party->resources + 624), party->leader->entity.secondary_resource,
                        party->leader->entity.palette_resource, 256);
    auxiliary->entity.render_object->state_flag_bits.behavior_state = 1;
    func_020093b4(auxiliary->entity.render_object, 1);
    auxiliary->entity.render_object->sort_key = party->follower->entity.render_object->sort_key;
    renderer = party->follower->entity.render_object;
    auxiliary->entity.render_object->overlap_priority_bytes[1] =
        renderer->overlap_priority_bytes[0] > renderer->overlap_priority_bytes[1]
            ? renderer->overlap_priority_bytes[0]
            : renderer->overlap_priority_bytes[1];
    auxiliary->entity.render_object->overlap_priority_bytes[0] =
        auxiliary->entity.render_object->overlap_priority_bytes[1];
    func_ov000_020931b0(party, party->follower, 1);
    FieldVertical_Start(&party->follower->entity, 8192, 1076, -1);
    GameAudio_PlayEffectDelayed(54, 0, -1);
}
void FieldParty_BeginState78(FieldPartyController *party)
{
    FieldResourceContext *area;
    const FieldPrimaryResource *resource;
    FieldPartyEntity *leader;
    int index;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->leader->entity.locomotion_state = 78;
    area = party->areas[party->flags.field_screen];
    index = area->special_resources[1];
    resource = index != -1 ? &area->primary[0][index] : 0;
    FieldEntity_RebindRendererResources(&party->leader->entity,
        (const FieldPrimaryResource *)resource, 0, 0, -1, 1, 256);
    party->leader->presentation.unknown_05 = 1;
    party->leader->presentation.resource_index =
        party->areas[party->flags.field_screen]->special_resources[1];
    party->leader->entity.render_object->state_flag_bits.behavior_state = 1;
    leader = party->leader;
    if (!leader->presentation.behavior_saved) {
        leader->presentation.saved_behavior = (u16)leader->entity.saved_presentation_flag_bits.behavior_mode;
        party->leader->presentation.behavior_saved = 1;
    }
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 1;
    GameAudio_PlayEffectDelayed(211, 0, -1);
}
}

#define LEADER party->leader
#define FOLLOWER party->follower
#define CURRENT_AREA party->areas[party->flags.field_screen]
#define PREVIOUS_AREA party->areas[party->flags.previous_field_screen]
static inline void RestoreBehavior(FieldPartyController *party, int member)
{
    if (party->members[member]->presentation.behavior_saved) {
        party->members[member]->entity.saved_presentation_flag_bits.behavior_mode =
            party->members[member]->presentation.saved_behavior;
        party->members[member]->presentation.behavior_saved = 0;
    } else
        party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 3;
}
extern "C" void FieldParty_LaunchAuxiliary(FieldPartyController *party)
{
    int index = party->unknown_054 % 6;
    --party->unknown_054;
    if (party->unknown_054) {
        if (party->unknown_094_render)
            party->unknown_094_render->set_animation(party->unknown_054, 0, 1);
    } else
        FieldParty_SetAttachedSpriteVisible(party, 0);
    FieldResourceContext *area = PREVIOUS_AREA;
    int resource_index = area->special_resources[2];
    const FieldPrimaryResource *resource = resource_index != -1 ? &area->primary[0][resource_index] : 0;
    fx32 x, y;
    FieldGeometry_GetDirectionVector(LEADER->entity.base_state_flag_bits.facing_direction, 40960, &x, &y);
    FieldAuxiliaryEntity *aux = LEADER->auxiliaries[index];
    aux->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    aux->entity.base_state_flag_bits.animation_wait_enabled = 1;
    aux->entity.runtime_flag_bits.sync_horizontal = 1;
    aux->entity.runtime_flag_bits.sync_vertical = 1;
    aux->entity.runtime_flag_bits.unknown_02 = 1;
    aux->entity.unknown_3c8 = 0;
    aux->entity.unknown_3c8 |= 4096;
    aux->entity.runtime_flag_bits.auto_priority_0 = 1;
    aux->entity.collision_state_flag_bits.unknown_04 = 1;
    aux->entity.collision_state_flag_bits.unknown_06 = 1;
    aux->entity.collision_state_flag_bits.unknown_07 = 1;
    aux->entity.field_state_flag_bits.body_collision_enabled = 1;
    aux->bits.follow_position = 0;
    aux->bits.follow_direction = 0;
    aux->target = 0;
    aux->bits.direction_mode = 0;
    aux->bits.unknown_04 = 0;
    aux->bits.unknown_05_06 = 0;
    aux->bits.unknown_07_08 = 0;
    aux->bits.follow_locomotion = 0;
    aux->entity.collision_extra_bits.unknown_20 = 1;
    aux->entity.locomotion_state = 94;
    FieldEntity3D_SetPosition(&aux->entity, LEADER->entity.position_x, LEADER->entity.position_y,
                              LEADER->entity.position_z);
    aux->bits.unknown_19 = 1;
    aux->bits.resource_index = PREVIOUS_AREA->special_resources[2];
    aux->bits.unknown_28_31 = -1;
    aux->offset_x = 0;
    aux->offset_y = 0;
    aux->offset_z = 0;
    aux->previous_offset_x = 0;
    aux->previous_offset_y = 0;
    aux->previous_offset_z = 0;
    aux->owner_offset_x = 0;
    aux->owner_offset_y = 0;
    aux->owner_offset_z = 0;
    aux->entity.animation_id = LEADER->entity.base_state_flag_bits.facing_direction;
    FieldEntity_ConfigureRendererResources(&aux->entity,
        (const FieldPrimaryResource *)resource, LEADER->entity.secondary_resource,
                        LEADER->entity.palette_resource, 256);
    aux->entity.render_object->state_flag_bits.behavior_state = 0;
    func_020093b4(aux->entity.render_object, 1);
    FieldEntity_SetFacingDirection(&aux->entity, 0, LEADER->entity.base_state_flag_bits.facing_direction, 1);
    FieldLinear3D_Start(&aux->entity, 1, 16 * x, 16 * y, 0, 18432, 0, 18432, -18432, 15, 15, 1, 0);
}
extern "C" void FieldParty_FinishAuxiliaryLaunch(FieldPartyController *party)
{
    party->state.unknown_01 = 1;
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 1;
    if (!party->unknown_054) {
        party->unknown_06c = 65536;
        FieldParty_ResetActionState(party, 1, 0);
    } else {
        FieldResourceContext *area = PREVIOUS_AREA;
        int index = area->special_resources[0];
        const FieldPrimaryResource *resource = index != -1 ? &area->primary[0][index] : 0;
        FieldEntity_RebindRendererResources(&LEADER->entity,
            (const FieldPrimaryResource *)resource, 0, 0, -1, 1, 128);
        LEADER->presentation.unknown_05 = 1;
        LEADER->presentation.resource_index = PREVIOUS_AREA->special_resources[0];
        RestoreBehavior(party, 0);
        LEADER->entity.locomotion_state = 0;
    }
}
extern "C" void FieldAuxiliary_StopSpecialRenderer(FieldAuxiliaryEntity *aux)
{
    aux->bits.resource_index = -1;
    FieldEntity_StopRenderer(&aux->entity);
}
extern "C" void FieldParty_FinishState76(FieldPartyController *party)
{
    func_ov000_02092e24(party, FOLLOWER);
    FOLLOWER->entity.locomotion_state = 3;
    RestoreBehavior(party, 1);
    FOLLOWER->entity.field_state_flag_bits.unknown_16 = 0;
    FOLLOWER->auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 0;
    FOLLOWER->auxiliary->entity.base.set_visible(0);
    FOLLOWER->auxiliary->entity.base.unknown_30();
    if (FOLLOWER->state_record->member_action.flags.area_value_saved) {
        CURRENT_AREA->unknown_2458 = FOLLOWER->state_record->member_action.saved_area_value;
        FOLLOWER->state_record->member_action.flags.area_value_saved = 0;
    }
    FOLLOWER->state_record->unknown_00[0] &= ~0x7ff;
}
extern "C" void FieldParty_SetAttachedSpriteVisible(FieldPartyController *party, int visible)
{
    if (visible) {
        if (party->unknown_054) {
            FieldResourceContext *area = party->areas[party->state_bits.unknown_05];
            int index = area->special_resources[5];
            FieldRuntimeEntity *entity = index != -1 ? area->entities[index] : 0;
            entity->base.set_visible(0);
            entity->base.property_00a_bits.property_00a_flag_00 = 0;
            s16 x = party->indicator_x + party->indicator_offset_x;
            s16 y = party->indicator_y + party->indicator_slide_y + party->indicator_offset_y;
            party->unknown_094_render = entity->render_object;
            party->unknown_094_render->set_animation(party->unknown_054, 0, 1);
            party->unknown_094_render->unknown_060 =
                (u8)(party->flags.unknown_03 ? party->state_bits.unknown_14_17
                                             : party->state_bits.movement_mode);
            FieldRenderObject *renderer = party->unknown_094_render;
            renderer->animation_offset_x = x - 10;
            renderer->animation_offset_y = y + 17;
            func_020093b4(party->unknown_094_render, (u8)party->indicator_bits.visible);
            party->state_bits.unknown_13 = 1;
        }
    } else if (party->unknown_094_render) {
        func_020093b4(party->unknown_094_render, 0);
        party->unknown_094_render->unknown_060 = (u8)party->state_bits.unknown_14_17;
        party->unknown_094_render = 0;
        party->state_bits.unknown_13 = 0;
    }
}
extern "C" void FieldParty_BeginAuxiliaryPlacement(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    LEADER->entity.movement_speed = 0;
    FOLLOWER->entity.movement_speed = 0;
    party->state.unknown_01 = 0;
    FieldParty_SetAttachedSpriteVisible(party, 0);
    if (FOLLOWER->entity.locomotion_state >= 76U && FOLLOWER->entity.locomotion_state <= 77U)
        FieldParty_FinishState76(party);
    LEADER->state_record->auxiliary_launch.elapsed_frames = 0;
    LEADER->entity.locomotion_state = 80;
    FieldResourceContext *area = CURRENT_AREA;
    int resource_index = area->special_resources[3];
    const FieldPrimaryResource *resource = resource_index == -1 ? 0 : &area->primary[0][resource_index];
    FieldEntity_RebindRendererResources(&LEADER->entity,
        (const FieldPrimaryResource *)resource, 0, 0, -1, 1, 256);
    LEADER->presentation.unknown_05 = 1;
    LEADER->presentation.resource_index = CURRENT_AREA->special_resources[3];
    LEADER->entity.render_object->state_flag_bits.behavior_state = 1;
    if (!LEADER->presentation.behavior_saved) {
        LEADER->presentation.saved_behavior = (u16)LEADER->entity.saved_presentation_flag_bits.behavior_mode;
        LEADER->presentation.behavior_saved = 1;
    }
    LEADER->entity.saved_presentation_flag_bits.behavior_mode = 1;
    area = CURRENT_AREA;
    resource_index = area->special_resources[3];
    resource = resource_index != -1 ? &area->primary[0][area->special_resources[4]] : 0;
    int index = party->unknown_054 % 6;
    FieldAuxiliaryEntity *aux = LEADER->auxiliaries[index];
    aux->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    aux->entity.base_state_flag_bits.animation_wait_enabled = 1;
    aux->entity.runtime_flag_bits.sync_horizontal = 0;
    aux->entity.runtime_flag_bits.sync_vertical = 0;
    aux->entity.runtime_flag_bits.unknown_02 = 0;
    aux->entity.unknown_3c8 = 0;
    aux->entity.unknown_3c8 &= ~4096;
    aux->entity.runtime_flag_bits.auto_priority_0 = 0;
    aux->entity.collision_state_flag_bits.unknown_04 = 0;
    aux->entity.collision_state_flag_bits.unknown_06 = 0;
    aux->entity.collision_state_flag_bits.unknown_07 = 0;
    aux->entity.field_state_flag_bits.body_collision_enabled = 1;
    aux->bits.follow_position = 1;
    aux->bits.follow_direction = 1;
    aux->target = &LEADER->entity;
    aux->bits.direction_mode = 1;
    aux->bits.unknown_04 = 0;
    aux->bits.unknown_05_06 = 0;
    aux->bits.unknown_07_08 = 0;
    aux->bits.follow_locomotion = 1;
    aux->entity.locomotion_state = 80;
    aux->bits.unknown_19 = 1;
    aux->bits.resource_index = CURRENT_AREA->special_resources[4];
    aux->bits.unknown_28_31 = -1;
    aux->owner_offset_x = data_ov000_020c07f8[LEADER->entity.base_state_flag_bits.facing_direction][0];
    aux->owner_offset_y = data_ov000_020c07fc[LEADER->entity.base_state_flag_bits.facing_direction][0];
    aux->owner_offset_z = 0;
    aux->entity.animation_id = LEADER->entity.base_state_flag_bits.facing_direction;
    FieldEntity_ConfigureRendererResources(&aux->entity,
        (const FieldPrimaryResource *)resource, LEADER->entity.secondary_resource,
                        LEADER->entity.palette_resource, 256);
    aux->entity.render_object->state_flag_bits.behavior_state = 1;
    func_020093b4(aux->entity.render_object, 1);
    LEADER->state_record->auxiliary_launch.auxiliary_index = index;
    party->unknown_054 = 0;
    GameAudio_PlayEffectDelayed(320, 0, -1);
}

#undef LEADER
#undef FOLLOWER
#undef CURRENT_AREA
#undef PREVIOUS_AREA
