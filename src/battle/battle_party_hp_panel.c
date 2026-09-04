#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <game/save_data.h>

enum BattlePartyHpPanelConstant {
    BATTLE_PARTY_HP_PANEL_OBJECT_OFFSET = 0x65E8,
    BATTLE_PARTY_HP_PANEL_TEXTURE_FRAME_OFFSET = 0x68D0,
    BATTLE_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_VIEW_Y_OFFSET = 0xCB9E,
    SAVE_PARTY_FORM_OFFSET = 0x558
};

typedef struct BattlePartyHpPanelNumberEffect {
    u8 unknown_00[0x16];
    s16 y_offset;
} BattlePartyHpPanelNumberEffect;

typedef struct BattlePartyHpPanelAnchor {
    u8 unknown_00[0x0E];
    s16 x;
    s16 y;
    s16 size;
} BattlePartyHpPanelAnchor;

typedef struct BattlePartyHpPanelState {
    BattleSceneObject *scene_object;
    s16 intensity;
    s16 visible;
    u16 actor_id;
    s16 displayed_hp;
    u8 unknown_0c[4];
    s16 y;
    s16 layout;
    BattlePartyHpPanelNumberEffect *number_effect;
    BattlePartyHpPanelAnchor *anchor;
} BattlePartyHpPanelState;

typedef union BattlePartyHpPanelTransform {
    BattleSpriteTransform value;
    s32 words[16];
} BattlePartyHpPanelTransform;

typedef struct BattlePartyHpPanelDisplayList {
    u8 unknown_00[0x50];
    u8 *start;
    u8 *end;
} BattlePartyHpPanelDisplayList;

extern const BattlePartyHpPanelTransform gBattlePartyHpPanelTransformTemplate;

extern void func_0202cc58(const void *source, void *destination, u32 size);
extern void func_0202cd2c(const void *source, void *destination, u32 size);
extern void DC_FlushRange(const void *start, u32 size);
extern void func_01ff861c(int channel, void *source, u32 size);

void BattlePartyHpPanel_Draw(BattlePartyHpPanelState *state) {
    BattlePartyHpPanelTransform transform;
    int intensity;
    BattlePartyActor *actor;
    int actor_frame;
    BattleObjectDataLoadState *load_state;
    u8 *destination;
    u8 *transform_destination;
    u32 display_list_size;
    int aligned_x;
    int aligned_y;

    intensity = state->intensity;
    if (intensity < 1 || state->actor_id == 0) {
        return;
    }

    actor = (BattlePartyActor *)BattleActor_GetPartySlot(state->actor_id);
    switch (actor->formation_index) {
    case 0:
        actor_frame = 10;
        break;
    case 1:
        actor_frame = 11;
        break;
    case 2:
        actor_frame = 28;
        break;
    case 3:
        actor_frame = 29;
        break;
    case 4:
        actor_frame = 10;
        break;
    case 5:
        actor_frame = 11;
        break;
    default:
        return;
    }

    transform = gBattlePartyHpPanelTransformTemplate;
    transform.value.y = state->y << 8;
    if (state->layout == 2) {
        int current_x;
        int current_y;

        if (*(s16 *)(gSaveData + SAVE_PARTY_FORM_OFFSET) == 2 &&
            (u16)(actor->formation_index + 0xFFFE) <= 1) {
            transform.value.x += 27136;
        }
        BattleSprite_DrawFrame(
            actor_frame, intensity, &transform.value,
            (int)state->scene_object, 8, 3, 0x7FFF);
        /* Preserve the original independent field-update schedule. */
        asm {
            ldr r2, [sp, #0x3C]
            ldr r0, [sp, #0x44]
            ldr r1, [sp, #0x40]
            add r2, r2, #0xC00
            sub r0, r0, #2
            sub r1, r1, #0x800
            str r2, [sp, #0x3C]
            str r0, [sp, #0x44]
            str r1, [sp, #0x40]
        }

        if (state->number_effect != 0) {
            state->number_effect->y_offset = -8;
        } else {
            BattleNumber_DrawDecimal(
                state->displayed_hp, intensity, &transform.value,
                0, 0x7FFF, 0, 0);
        }

        current_x = transform.value.x;
        current_y = transform.value.y;
        if (state->displayed_hp > 99) {
            current_x -= 2048;
        } else if (state->displayed_hp > 9) {
            current_x -= 1024;
        }

        transform.value.x += 2048;
        transform.value.y += 2816;
        BattleNumber_DrawDecimal(
            actor->actor.max_hp, intensity, &transform.value,
            0, 0x7FFF, 42, 0);
        if (actor->actor.max_hp > 99) {
            current_x -= 2048;
        } else if (actor->actor.max_hp > 9) {
            current_x -= 1024;
        }

        /* Keep MWCC from folding each center into its source register. */
        asm {
            ldr r3, [sp, #0x3C]
            ldr r0, [sp, #0x40]
            add r1, current_x, r3
            sub r4, r3, #0x1800
            add r2, current_y, r0
            add r3, r0, #0x200
            add r1, r1, r1, lsr #31
            add r2, r2, r2, lsr #31
            str r4, [sp, #0x3C]
            mov r0, #3
            str r3, [sp, #0x40]
            mov aligned_x, r1, asr #1
            mov aligned_y, r2, asr #1
            bl BattleObjectData_GetLoadState
            mov load_state, r0
        }
        destination =
            ((BattlePartyHpPanelDisplayList *)load_state->data)->start;
        transform_destination = destination + 12;
        if ((u8 *)&transform < transform_destination) {
            func_0202cd2c(
                &transform, transform_destination, sizeof(transform));
        } else {
            func_0202cc58(
                &transform, transform_destination, sizeof(transform));
        }
        *(u32 *)(destination + 0x4C) =
            (*(u32 *)(destination + 0x4C) & 0xC0E0FFFF) |
            (*(u16 *)(gBattleContext +
                      BATTLE_PARTY_HP_PANEL_TEXTURE_FRAME_OFFSET) << 24) |
            (intensity << 16);
        display_list_size =
            ((BattlePartyHpPanelDisplayList *)load_state->data)->end -
            ((BattlePartyHpPanelDisplayList *)load_state->data)->start;
        DC_FlushRange(destination, display_list_size);
        func_01ff861c(1, destination, display_list_size);
    } else {
        BattleSprite_DrawFrame(
            actor_frame, intensity, &transform.value,
            (int)state->scene_object, 8, 3, 0x7FFF);
        transform.value.x += 3072;
        transform.value.y -= 1024;
        transform.value.scale -= 2;
        if (state->number_effect == 0) {
            BattleNumber_DrawDecimal(
                state->displayed_hp, intensity, &transform.value,
                0, 0x7FFF, 0, 0);
        }

        aligned_x = transform.value.x;
        aligned_y = transform.value.y;
        if (state->displayed_hp > 99) {
            aligned_x -= 2048;
        } else if (state->displayed_hp > 9) {
            aligned_x -= 1024;
        }
    }

    if (state->anchor != 0) {
        state->anchor->x =
            aligned_x / 256 + 2 +
            *(s16 *)(gBattleContext + BATTLE_VIEW_X_OFFSET);
        state->anchor->y =
            aligned_y / 256 +
            *(s16 *)(gBattleContext + BATTLE_VIEW_Y_OFFSET);
        state->anchor->size = 8;
    }
}

void BattlePartyHpPanel_Update(BattlePartyHpPanelState *state) {
    int intensity;

    if (state->scene_object->primary_model == 0) {
        return;
    }

    intensity = state->intensity;
    if (state->visible != 0) {
        if (state->scene_object->primary_model->
                flag_bits.panel_animation_trigger != 0 &&
            state->scene_object->primary_model->animation_id == 0) {
            BattleSceneObject_SetAnimation(
                *(BattleSceneObject **)(gBattleContext +
                                         BATTLE_PARTY_HP_PANEL_OBJECT_OFFSET),
                1, -1);
        }

        if (intensity > 22) {
            intensity++;
        } else {
            intensity += 2;
        }
        if (intensity > 31) {
            intensity = 31;
        }
        state->intensity = intensity;
        state->scene_object->primary_model->animation_state_bits.state = 31;
        return;
    }

    if (intensity <= 0) {
        return;
    }
    intensity -= 3;
    if (intensity <= 0) {
        intensity = 0;
        BattleSceneObject_SetAnimation(state->scene_object, -1, -1);
    }
    state->scene_object->primary_model->animation_state_bits.state = intensity;
    state->intensity = state->scene_object->primary_model->
        animation_state_bits.state;
}
