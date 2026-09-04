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
    BATTLE_COMMAND_WHEEL_BOUNCE_FRAMES = 12
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
