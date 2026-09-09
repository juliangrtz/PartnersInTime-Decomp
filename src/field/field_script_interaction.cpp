#include <game/field_roaming.h>
extern "C" {
#include <nitro/fx.h>
}
#include <game/field_script_context.h>
#include <game/field_presentation.h>
#include <game/field_entity_motion.h>
extern "C" {

void FieldScript_BeginInteraction(FieldVmRuntime *runtime, FieldRuntimeEntity *actor,
                                  FieldRuntimeEntity *target, FieldScriptState *state)
{
    u32 contacts;
    state->owner_data[0] = actor->base.index;
    state->owner_data[1] = target->base.index;
    contacts = actor->unknown_3a0_bits.contact_mask_b;
    if (contacts & 1)
        state->owner_data[2] = 4;
    else if (contacts & 4)
        state->owner_data[2] = 0;
    else if (contacts & 8)
        state->owner_data[2] = 2;
    else if (contacts & 2)
        state->owner_data[2] = 6;
    else if (contacts & 16)
        state->owner_data[2] = 9;
    else if (contacts & 32)
        state->owner_data[2] = 8;
    state->flag_bits.owner_subtype = actor->locomotion_category;
    if (target->base.state_flag_bits.active) {
        target->base.pause_script();
        target->base.state_flag_bits.unknown_flag_03 = 1;
    }
    if (target->roaming_state.active)
        FieldRoaming_Pause(target);
    target->field_state_flag_bits.unknown_08_10 = target->base_state_flag_bits.facing_direction;
    if (target->field_state_flag_bits.turn_to_interactor)
        FieldEntity_FaceTarget(target, actor);
}

void FieldScript_EndInteraction(FieldVmRuntime *runtime, FieldScriptState *state)
{
    FieldScriptContext *field = (FieldScriptContext *)runtime->field_context;
    FieldRuntimeEntity *entity = (FieldRuntimeEntity *)field->entities[(s8)state->owner_data[1]];
    if (entity->base.state_flag_bits.active) {
        if (entity->base.state_flag_bits.flag_02 && entity->base.state_flag_bits.unknown_flag_03) {
            entity->base.resume_script();
            entity->base.state_flag_bits.unknown_flag_03 = 0;
        }
    } else {
        if (entity->field_state_flag_bits.turn_to_interactor ||
            entity->saved_presentation_flag_bits.behavior_mode >= 3)
            FieldEntity_SetFacingDirection(entity, 0, entity->field_state_flag_bits.unknown_08_10, 1);
    }
}
}
