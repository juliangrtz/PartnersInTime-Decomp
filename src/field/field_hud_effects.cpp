extern "C" {
#include <nitro/fx.h>
#include <game/rumble.h>
#include <game/save_data.h>
}
#include <game/field_hud.h>
extern "C" {
extern void func_020093b4(FieldRenderObject *, int);
extern GameOamEntry *func_ov000_02071de8(void *, GameOamEntry *, s16, int, int, int, u32);
enum { SAVE_PARTY_OFFSET = 1016, SAVE_COINS_OFFSET = 1160 };

static inline void FieldHud_Position(FieldRenderObject *renderer, s16 x, s16 y)
{
    renderer->animation_offset_x = x;
    renderer->animation_offset_y = y;
}

GameOamEntry *FieldHud_Draw(void *field_context, GameOamEntry *output)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    if (field->hud.visible || field->hud.moving) {
        int single = field->party->groups[field->party->active_group].active_members == 1;
        if (single)
            func_020093b4(&field->renderers[8]->animation.base, 0);
        if (field->hud.moving) {
            field->hud_y += field->hud_step;
            if (field->hud_step > 0) {
                if (field->hud_y >= 52) {
                    field->hud_y = 52;
                    field->hud.moving = 0;
                    field->hud.visible = 0;
                    FieldHud_SetRenderersVisible(field, 0, 0);
                }
            } else if (field->hud_y <= 0) {
                field->hud_y = 0;
                field->hud.moving = 0;
                field->hud.visible = 1;
            }
            FieldHud_UpdatePositions(field, single);
        }
        if (single) {
            output = func_ov000_02071de8(field, output, 17, (s16)(field->hud_y + 175),
                                         ((SavePartyMember *)(gSaveData + 72 * field->party->active_group + SAVE_PARTY_OFFSET))[0].current_hp, 3,
                                         field->digit_graphics);
            output = func_ov000_02071de8(field, output, 68, (s16)(field->hud_y + 175),
                                         *(u32 *)(gSaveData + SAVE_COINS_OFFSET), 6, field->digit_graphics);
        } else {
            output = func_ov000_02071de8(field, output, 65, (s16)(field->hud_y + 175),
                                         ((SavePartyMember *)(gSaveData + 72 * field->party->active_group + SAVE_PARTY_OFFSET))[0].current_hp, 3,
                                         field->digit_graphics);
            output = func_ov000_02071de8(field, output, 17, (s16)(field->hud_y + 175),
                                         ((SavePartyMember *)(gSaveData + 72 * field->party->active_group + SAVE_PARTY_OFFSET))[1].current_hp, 3,
                                         field->digit_graphics);
            output = func_ov000_02071de8(field, output, 116, (s16)(field->hud_y + 175),
                                         *(u32 *)(gSaveData + SAVE_COINS_OFFSET), 6, field->digit_graphics);
        }
    }
    return output;
}

void FieldHud_UpdatePositions(void *field_context, int single)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    if (single) {
        FieldHud_Position(&field->renderers[7]->animation.base, 4, field->hud_y + 174);
        FieldHud_Position(&field->renderers[9]->animation.base, 28, field->hud_y + 184);
        FieldHud_Position(&field->renderers[6]->animation.base, 52, field->hud_y + 174);
    } else {
        FieldHud_Position(&field->renderers[7]->animation.base, 52, field->hud_y + 174);
        FieldHud_Position(&field->renderers[9]->animation.base, 76, field->hud_y + 184);
        FieldHud_Position(&field->renderers[8]->animation.base, 4, field->hud_y + 174);
        FieldHud_Position(&field->renderers[10]->animation.base, 28, field->hud_y + 184);
        FieldHud_Position(&field->renderers[6]->animation.base, 100, field->hud_y + 174);
    }
}

int FieldEffect_Start(void *field_context, int slot, int animation, s16 x, s16 y, int duration)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    if (slot == -1) {
        int i;
        for (i = 0; i < 8; ++i) {
            if (!field->renderers[i + 15]->animation.base.state_flag_bits.animation_active) {
                slot = i;
                break;
            }
        }
        if (slot == -1)
            slot = 0;
    }
    FieldTimedRenderer *renderer = field->renderers[slot + 15];
    renderer->animation.base.animation_offset_x = x;
    renderer->animation.base.animation_offset_y = y;
    renderer->animation.base.set_animation(animation, 0, 1);
    func_020093b4(&renderer->animation.base, 1);
    FieldTimedRenderer_ResetAnimationControl(&renderer->animation);
    if (duration < 0) {
        FieldTimedRenderer_SetLoopCount(&renderer->animation, -duration);
        renderer->state.halves.remaining = 0;
        renderer->state.bits.expired = 0;
    } else {
        renderer->state.halves.remaining = duration;
        renderer->state.bits.expired = 0;
    }
    return slot;
}

void FieldEffect_Stop(void *field_context, int slot)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    func_020093b4(&field->renderers[slot + 15]->animation.base, 0);
    field->effect_owners[slot] = 255;
}

void FieldEffect_StopAll(void *field_context)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    int i;
    for (i = 0; i < 8; ++i)
        FieldEffect_Stop(field, i);
}

int FieldEffect_AnyActive(void *field_context)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    int i;
    for (i = 0; i < 8; ++i) {
        if (field->renderers[i + 15]->animation.base.state_flag_bits.animation_active)
            return 1;
    }
    return 0;
}

int FieldEffect_StartForEntity(void *field_context, FieldRuntimeEntity *entity, int slot, int animation,
                               int relative, s16 x, s16 y, int duration, u8 follow)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    if (relative == 1) {
        x += entity->unknown_1a6[0];
        y += entity->unknown_1a6[1] +
             (entity->interaction_min_y - entity->interaction_vertical_extent) / 4096 - 2;
    }
    if (slot == -1)
        slot = FieldEffect_FindSlot(field, entity);
    int index = FieldEffect_Start(field, slot, animation, x, y, duration);
    FieldTimedRenderer *renderer = field->renderers[index + 15];
    struct FollowFlag {
        u32 absolute : 1, reserved : 31;
    };
    ((FollowFlag *)&renderer->state)->absolute = follow ^ 1;
    field->effect_owners[index] = entity->base.index | (follow << 7);
    return index;
}

void FieldEffect_StopForEntity(void *field_context, FieldRuntimeEntity *entity)
{
    FieldHudContext *field = (FieldHudContext *)field_context;
    int index = FieldEffect_FindSlot(field, entity);
    if (index != -1)
        FieldEffect_Stop(field, index);
}
}
