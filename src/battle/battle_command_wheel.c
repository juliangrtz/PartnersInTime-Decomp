#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

enum BattleCommandWheelConstant {
    BATTLE_COMMAND_WHEEL_OFFSET = 0x6530,
    BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET = 0x6538,
    BATTLE_TARGET_CURSOR_ANIMATION_OFFSET = 0x6574,
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_SELECTED_COMMAND_OFFSET = 0x11A,
    BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET = 0x12E,
    BATTLE_COMMAND_WHEEL_MAX_ENTRIES = 5,
    BATTLE_COMMAND_WHEEL_OPEN_RADIUS = 0x2400,
    BATTLE_COMMAND_WHEEL_RADIUS_STEP = 0x480,
    BATTLE_COMMAND_WHEEL_ENTRY_LIFT = 0x400,
    BATTLE_COMMAND_WHEEL_ENTRY_LIFT_STEP = 0x66,
    BATTLE_COMMAND_WHEEL_BOUNCE_FRAMES = 12,
    BATTLE_COMMAND_WHEEL_SORT_CAPACITY = 8,
    BATTLE_COMMAND_WHEEL_RENDER_RESOURCE_ID = 3,
    BATTLE_COMMAND_WHEEL_FULL_INTENSITY = 31
};

enum BattleCommandWheelPhase {
    BATTLE_COMMAND_WHEEL_PHASE_ATTACH = 1,
    BATTLE_COMMAND_WHEEL_PHASE_OPEN = 2,
    BATTLE_COMMAND_WHEEL_PHASE_ACTIVE = 3
};

typedef union BattleCommandWheelEntryFlags {
    u16 raw;
    struct {
        s16 icon_id : 15;
        u16 dismiss_after_bounce : 1;
    } bits;
} BattleCommandWheelEntryFlags;

typedef struct BattleCommandWheelEntry {
    s16 angle;
    s16 lift;
    s8 spin_angle;
    s8 bounce_timer;
    BattleCommandWheelEntryFlags flags;
} BattleCommandWheelEntry;

typedef union BattleCommandWheelFlags {
    u16 raw;
    struct {
        u16 moving : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleCommandWheelFlags;

typedef struct BattleCommandWheelState {
    BattleSceneObject *scene_object;
    s16 intensity;
    s16 phase;
    BattleCommandWheelEntry entries[BATTLE_COMMAND_WHEEL_MAX_ENTRIES];
    s16 entry_count;
    s16 angular_speed;
    s16 radius;
    BattleCommandWheelFlags flags;
} BattleCommandWheelState;

typedef union BattleCommandWheelTransform {
    BattleSpriteTransform value;
    s32 words[16];
} BattleCommandWheelTransform;

extern s16 FX_SinCosTable_[8192];
extern const BattleCommandWheelTransform
    gBattleCommandWheelTransformTemplate;
extern const s8 gBattleCommandWheelFrames[];
extern void func_ov002_020a2fd8(int resource_id, int intensity,
                                int x, int y, int z,
                                int render_flags);

int BattleCommandWheel_TriggerEntryBounce(int dismiss_after_bounce) {
    BattleCommandWheelEntry *entry =
        &((BattleCommandWheelEntry *)(
              gBattleContext + BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET))[
            *(s16 *)(gBattleContext + BATTLE_SELECTED_COMMAND_OFFSET)];

    if (entry->bounce_timer == 0) {
        (*(s16 *)(gBattleContext +
                  BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET))++;
    }
    entry->bounce_timer = BATTLE_COMMAND_WHEEL_BOUNCE_FRAMES;
    entry->flags.bits.dismiss_after_bounce = dismiss_after_bounce != 0;
    return BattleSound_Play(9, 0, 0, 0);
}

void BattleCommandWheel_Update(BattleCommandWheelState *state,
                               int selected_index) {
    BattleCommandWheelEntry *entries;
    int entry_count;
    int angular_speed;

    if (*(s8 *)(gBattleContext +
                BATTLE_TARGET_CURSOR_ANIMATION_OFFSET) != 0) {
        return;
    }
    if (*(s16 *)(gBattleContext +
                 BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET) != 0 &&
        state->phase != 0 && state->intensity < 31) {
        return;
    }
    if (BattleRender_UpdateIntensity(state->phase, &state->intensity) == 0) {
        return;
    }

    entries = state->entries;
    state->flags.bits.moving = 0;
    entry_count = state->entry_count;
    angular_speed = state->angular_speed;
    switch (state->phase) {
    case BATTLE_COMMAND_WHEEL_PHASE_ATTACH: {
        BattleSceneObject *actor_object = BattleSceneObject_GetById(
            *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET));
        int target_z = 0;
        int i;

        switch (((BattlePartyActor *)BattleActor_GetPartySlot(
                     *(u16 *)(gBattleContext +
                              BATTLE_ACTIVE_ACTOR_ID_OFFSET)))->
                    formation_index) {
        case 0:
        case 4:
            target_z = 64;
            break;
        case 1:
        case 5:
            target_z = 64;
            break;
        case 2:
            target_z = 48;
            break;
        case 3:
            target_z = 48;
            break;
        }
        BattleSceneObject_AdjustPosition(
            state->scene_object,
            actor_object->x - state->scene_object->x,
            actor_object->y - state->scene_object->y,
            target_z - state->scene_object->z);

        state->radius = 0;
        for (i = 0; i < entry_count; i++) {
            entries[i].angle =
                (entry_count / 2 + ((i - selected_index) << 8)) /
                entry_count;
        }
        state->intensity = 1;
        state->flags.bits.moving = 1;
        state->phase++;
        break;
    }

    case BATTLE_COMMAND_WHEEL_PHASE_OPEN:
        state->radius += BATTLE_COMMAND_WHEEL_RADIUS_STEP;
        if (state->radius >= BATTLE_COMMAND_WHEEL_OPEN_RADIUS) {
            state->radius = BATTLE_COMMAND_WHEEL_OPEN_RADIUS;
        }
        state->intensity =
            31 * state->radius / BATTLE_COMMAND_WHEEL_OPEN_RADIUS;
        state->flags.bits.moving = 1;
        if (state->radius == BATTLE_COMMAND_WHEEL_OPEN_RADIUS) {
            state->phase++;
        }
        break;

    case BATTLE_COMMAND_WHEEL_PHASE_ACTIVE: {
        int i;

        for (i = 0; i < entry_count; i++) {
            int target_angle =
                (entry_count / 2 + ((i - selected_index) << 8)) /
                entry_count;
            int angle = entries[i].angle;

            if (angle != target_angle) {
                state->flags.bits.moving = 1;
                if (angle + angular_speed < target_angle) {
                    angle += angular_speed;
                } else if (target_angle < angle - angular_speed) {
                    angle -= angular_speed;
                } else if (angle < target_angle || target_angle < angle) {
                    angle = target_angle;
                }
                entries[i].angle = angle;
            }

            if ((entries[i].flags.bits.icon_id & 1) != 0) {
                if ((target_angle & 0xFE) != 0) {
                    if (entries[i].lift > 0) {
                        entries[i].lift -=
                            BATTLE_COMMAND_WHEEL_ENTRY_LIFT_STEP;
                    }
                } else {
                    entries[i].lift = BATTLE_COMMAND_WHEEL_ENTRY_LIFT;
                }
                if (entries[i].lift < 0) {
                    entries[i].lift = 0;
                }
                if (entries[i].lift != 0) {
                    entries[i].spin_angle += 4;
                } else {
                    entries[i].spin_angle = 0;
                }
            } else {
                entries[i].lift = 0;
                entries[i].spin_angle = 0;
            }
        }
        break;
    }
    }
}

int BattleCommandWheel_Draw(BattleCommandWheelState *state) {
    s16 sorted_indices[BATTLE_COMMAND_WHEEL_SORT_CAPACITY];
    s16 sort_depth[BATTLE_COMMAND_WHEEL_SORT_CAPACITY];
    int entry_count = state->entry_count;
    int radius = state->radius;
    int draw_index;

    if (state->intensity < BATTLE_COMMAND_WHEEL_FULL_INTENSITY) {
        int i;

        for (i = 0; i < entry_count; i++) {
            int insert_at = 0;
            int angle_index =
                ((u16)(state->entries[i].angle << 8) >> 4) * 2;
            int depth = FX_SinCosTable_[angle_index + 1];

            sorted_indices[i] = i;
            sort_depth[i] = depth;
            if (i > 0) {
                while (insert_at < i &&
                       sort_depth[insert_at] <= depth) {
                    insert_at++;
                }
                if (insert_at < i) {
                    int shift;

                    for (shift = i; shift > insert_at; shift--) {
                        sorted_indices[shift] =
                            sorted_indices[shift - 1];
                        sort_depth[shift] = sort_depth[shift - 1];
                    }
                    sorted_indices[insert_at] = i;
                    sort_depth[insert_at] = depth;
                }
            }
        }
    }

    if (entry_count > 0) {
        int perspective_radius = 5 * radius / 256;

        for (draw_index = 0; draw_index < entry_count; draw_index++) {
            int intensity = state->intensity;
            int entry_index =
                intensity < BATTLE_COMMAND_WHEEL_FULL_INTENSITY
                    ? sorted_indices[draw_index]
                    : draw_index;
            BattleCommandWheelEntry *entry =
                &state->entries[entry_index];
            int should_draw = intensity >= 1;

            if (entry->bounce_timer != 0) {
                should_draw = 1;
                entry->bounce_timer--;
                if (entry->bounce_timer == 0) {
                    (*(s16 *)(gBattleContext +
                              BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET))--;
                    if (!entry->flags.bits.dismiss_after_bounce) {
                        entry->lift = 0;
                        entry->spin_angle = 0;
                        should_draw = 0;
                    }
                }
            }

            if (should_draw) {
                BattleSceneObject *object = state->scene_object;
                BattlePosition position;
                BattleCommandWheelTransform transform;
                int angle_index =
                    ((u16)(entry->angle << 8) >> 4) * 2;
                int horizontal_offset =
                    radius * FX_SinCosTable_[angle_index] / 0x100000;
                int depth_offset =
                    radius *
                    (FX_SinCosTable_[angle_index + 1] - 4096) /
                    0x100000;
                int spin_index =
                    ((u16)(entry->spin_angle << 8) >> 4) * 2;
                int lift_offset =
                    entry->lift * FX_SinCosTable_[spin_index] /
                    0x100000;
                int bounce_timer = entry->bounce_timer;
                int perspective_denominator;
                int perspective_depth;
                int perspective_x;
                int anchor_y;
                int shade;

                if (bounce_timer != 0) {
                    int bounce_step;

                    if (bounce_timer <= 1) {
                        bounce_timer = 2;
                    }
                    bounce_step = bounce_timer - 4;
                    lift_offset +=
                        48 * bounce_step / 8 -
                        48 * bounce_step * bounce_step / 64;
                }

                perspective_denominator =
                    perspective_radius - depth_offset;
                perspective_depth =
                    depth_offset * perspective_radius /
                    perspective_denominator;
                perspective_x =
                    horizontal_offset * perspective_radius /
                    perspective_denominator;
                anchor_y = object->y + 2 * perspective_depth / 3;

                BattlePosition_StoreViewRelative(
                    &position,
                    (s16)(object->x + perspective_x),
                    (s16)(anchor_y - (object->z + lift_offset)),
                    (s16)(object->effect_anchor_z +
                          16 * (256 - anchor_y)),
                    object->flags.bits.use_raw_position,
                    object->flags.bits.use_alternate_model);

                transform = gBattleCommandWheelTransformTemplate;
                transform.value.matrix[0] =
                    (perspective_radius << 12) /
                    perspective_denominator;
                transform.value.matrix[5] =
                    transform.value.matrix[0];
                transform.value.x = position.x << 8;
                transform.value.y = position.y << 8;
                transform.value.z = position.z - 4;

                if (1 - depth_offset >= 12) {
                    shade = 156 / (1 - depth_offset) + 18;
                } else {
                    shade = 31;
                }

                if (entry->bounce_timer != 0 &&
                    entry->bounce_timer <= 4) {
                    if (entry->flags.bits.dismiss_after_bounce) {
                        entry->bounce_timer = 0;
                        (*(s16 *)(gBattleContext +
                                  BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET))--;
                    } else {
                        transform.value.matrix[0] =
                            transform.value.matrix[0] *
                            (6 - entry->bounce_timer / 2) / 4;
                        transform.value.matrix[5] =
                            transform.value.matrix[5] *
                            entry->bounce_timer / 4;
                    }
                }
                if (entry->bounce_timer != 0) {
                    intensity = BATTLE_COMMAND_WHEEL_FULL_INTENSITY;
                }

                BattleSprite_DrawFrame(
                    gBattleCommandWheelFrames[
                        entry->flags.bits.icon_id],
                    intensity, &transform.value, 0, 0,
                    BATTLE_COMMAND_WHEEL_RENDER_RESOURCE_ID,
                    (shade & 0x1F) |
                        ((shade & 0x1F) << 5) |
                        ((shade & 0x1F) << 10));

                anchor_y = object->y + perspective_depth / 3;
                BattlePosition_StoreViewRelative(
                    &position,
                    (s16)(object->x + perspective_x),
                    (s16)anchor_y,
                    (s16)(object->effect_anchor_z +
                          16 * (256 - anchor_y)),
                    object->flags.bits.use_raw_position,
                    object->flags.bits.use_alternate_model);
                func_ov002_020a2fd8(
                    BATTLE_COMMAND_WHEEL_RENDER_RESOURCE_ID,
                    intensity, position.x, position.y,
                    object->z, 0);
            }
        }
    }

    return entry_count;
}
