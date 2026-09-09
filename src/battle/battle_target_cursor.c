#include <game/battle_selection_animation.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

typedef union BattleTargetCursorTransform {
    BattleSpriteTransform value;
    s32 words[16];
} BattleTargetCursorTransform;

extern const BattleTargetCursorTransform
    gBattleTargetCursorTransformTemplate;
extern s16 FX_SinCosTable_[8192];

static inline void BattleTargetCursor_StorePosition(
    BattleSceneObject *object, int y_offset, BattlePosition *position) {
    s16 cursor_y = object->y + y_offset;

    BattlePosition_StoreViewRelative(
        position, object->x, (s16)(cursor_y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - cursor_y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);
}





void BattleTargetCursor_Draw(BattleTargetCursorState *state) {
    BattlePosition position;
    BattleTargetCursorTransform transform;
    BattleSceneObject *object;
    int scale, y_offset, intensity;
    int timer;

    if (*(s16 *)(gBattleContext + BATTLE_INTERFACE_BLOCKED_OFFSET) != 0) {
        return;
    }
    if (state->visible == 0 &&
        state->animation.bytes.bounce_timer == 0) {
        return;
    }

    if (state->animation.bits.uniform_targets != 0) {
        scale = 4096;
        intensity = 31;
        y_offset =
            (*(s16 *)((u8 *)FX_SinCosTable_ +
                      4 * (((state->rotation_angle << 8) & 0xFFFF) >> 4)) <<
             10) /
            0x100000;
    } else {
        scale = 2560;
        y_offset = 0;
        intensity = 16;
    }

    timer = state->animation.bytes.bounce_timer;
    if (timer != 0) {
        state->animation.bytes.bounce_timer = timer - 1;
        timer = state->animation.bytes.bounce_timer;
        if (timer == 0) {
            return;
        }
    }
    if (timer != 0) {
        int bounce_step;
        int bounce_squared;

        if (timer <= 1) {
            timer = 2;
        }
        bounce_step = timer - 4;
        bounce_squared = bounce_step * bounce_step;
        y_offset -= 48 * bounce_step / 8 -
                    48 * bounce_squared / 64;
    }

    object = state->focus_object;
    BattleTargetCursor_StorePosition(object, y_offset, &position);

    transform = gBattleTargetCursorTransformTemplate;
    transform.value.x = position.x << 8;
    transform.value.y = position.y << 8;
    transform.value.z = position.z - 8;
    transform.value.matrix[0] = scale;
    transform.value.matrix[5] = scale;
    transform.value.matrix[10] = scale;

    if (state->animation.bytes.bounce_timer != 0 &&
        state->animation.bytes.bounce_timer <= 4) {
        transform.value.matrix[0] =
            scale * (6 - state->animation.bytes.bounce_timer / 2) / 4;
        transform.value.matrix[5] =
            scale * state->animation.bytes.bounce_timer / 4;
    }

    BattleSprite_DrawFrame(
        BATTLE_TARGET_CURSOR_FRAME, intensity, &transform.value,
        0, 0, 3, 0x7FFF);
}
