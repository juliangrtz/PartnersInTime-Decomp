#include <game/field_party.h>
extern "C" void func_ov000_020a6d68(FieldEntity *, const void *, int, int, int, int, int);
#define MEMBER party->members[member]
#define ENTITY MEMBER->entity

extern "C" void FieldParty_CopyAttachedRenderPriorities(FieldPartyController *party)
{
    int index = party->unknown_055;
    if (index == -1)
        return;
    FieldRuntimeEntity *source = party->areas[party->flags.field_screen]->entities[index];
    int member = 0;
    do {
        if (ENTITY.render_object->state_flag_bits.animation_active) {
            ENTITY.runtime_flag_bits.auto_priority_0 = 0;
            ENTITY.runtime_flag_bits.auto_priority_1 = 0;
            ENTITY.runtime_flag_bits.auto_priority_2 = 0;
            ENTITY.runtime_flag_bits.auto_priority_3 = 0;
            ENTITY.render_object->overlap_priority_bytes[0] =
                source->render_object->overlap_priority_bytes[1];
            ENTITY.render_object->overlap_priority_bytes[1] =
                source->render_object->overlap_priority_bytes[1];
            ENTITY.render_object->overlap_priority_bytes[2] =
                source->render_object->overlap_priority_bytes[1];
            ENTITY.render_object->overlap_priority_bytes[3] =
                source->render_object->overlap_priority_bytes[1];
        }
        ++member;
    } while (member < 2);
}

extern "C" void FieldParty_BindActionResource(FieldPartyController *party, FieldPartyEntity *member,
                                              int index, int speed, u8 restart)
{
    if (!member->presentation.unknown_00) {
        member->saved_animation_speed = member->entity.animation_speed;
        member->presentation.unknown_00 = 1;
    }
    func_ov000_020a6d68(&member->entity.base, (FieldPrimaryResource *)party->resources + index, 0, 0, -1, 1,
                        speed);
    if (restart)
        member->entity.render_object->set_animation((u8)member->entity.render_object->resource_animation, 0,
                                                    1);
    member->presentation.unknown_05 = 0;
    member->presentation.resource_index = (s16)index;
}
