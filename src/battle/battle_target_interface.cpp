#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_global_properties.h>
#include <game/battle_scene.h>

enum BattleTargetInterfaceConstant {
    BATTLE_TARGET_FOCUS_OBJECT_OFFSET = 0x6568,
    BATTLE_TARGET_INTERFACE_FLAGS_OFFSET = 0x6574,
    BATTLE_TARGET_INPUT_STATE_OFFSET = 0x130,
    BATTLE_TARGET_INPUT_FLAGS_OFFSET = 0x10
};

typedef union BattleTargetInterfaceFlags {
    u16 raw;
    struct {
        u16 unknown_00_07 : 8;
        u16 uniform_markers : 1;
        u16 unknown_09_15 : 7;
    } bits;
} BattleTargetInterfaceFlags;

typedef union BattleTargetInputFlags {
    u8 raw;
    struct {
        u8 confirm_held : 1;
        u8 unknown_01_07 : 7;
    } bits;
} BattleTargetInputFlags;

enum BattleTargetGroup {
    BATTLE_TARGET_GROUP_ENEMIES = 1,
    BATTLE_TARGET_GROUP_PARTY = 2
};

enum BattlePartyIndicatorAnimation {
    BATTLE_PARTY_INDICATOR_ADULT_FRONT = 12,
    BATTLE_PARTY_INDICATOR_ADULT_BACK = 13,
    BATTLE_PARTY_INDICATOR_BABY_FRONT = 14,
    BATTLE_PARTY_INDICATOR_BABY_BACK = 15,
    BATTLE_PARTY_INDICATOR_BABY_GROUP_A = 18,
    BATTLE_PARTY_INDICATOR_BABY_GROUP_B = 19
};

typedef struct BattleTargetOverlayState {
    BattleSceneObject *primary_marker;
    s16 intensity;
    s16 target_group;
    BattleSceneObject *secondary_marker;
    s16 animation_id;
} BattleTargetOverlayState;

typedef struct BattleTargetLabelState {
    BattleSceneObject *scene_object;
    s16 intensity;
    s16 requested_side;
    s16 displayed_side;
    s16 transition_angle;
    BattleInterfaceLayer layer;
} BattleTargetLabelState;

typedef union BattleTargetLabelTransform {
    BattleSpriteTransform value;
    s32 words[16];
} BattleTargetLabelTransform;

typedef char BattleTargetLabelState_SizeCheck[
    sizeof(BattleTargetLabelState) == 0x4C ? 1 : -1
];
typedef char BattleTargetLabelTransform_SizeCheck[
    sizeof(BattleTargetLabelTransform) == 0x40 ? 1 : -1
];

typedef struct BattlePartyIndicatorState {
    BattleSceneObject *object;
    s16 alpha;
    s16 enabled;
    u16 actor_id;
    u16 previous_party_group;
} BattlePartyIndicatorState;

extern "C" {

extern const BattleTargetLabelTransform gBattleTargetLabelTransformTemplate;
extern s16 FX_SinCosTable_[8192];

extern int func_ov002_020925bc(BattleSpriteTransform *transform,
                               BattleInterfaceLayer *layer);
extern void func_02036cc0(BattleSpriteTransform *transform);
extern int func_ov002_02093b88(void *task);
extern void *func_ov002_020725a4(int (*callback)(void *task),
                                 void *argument, int priority, int flags);

void BattleTargetLabel_Update(BattleTargetLabelState *state) {
    if (BattleRender_UpdateIntensity(
            state->requested_side, &state->intensity) == 0) {
        return;
    }

    if (state->displayed_side != state->requested_side) {
        if (state->transition_angle < 64) {
            state->transition_angle += 12;
        }
        if (state->transition_angle >= 64) {
            state->transition_angle = 64;
            if (state->layer.flags.bits.resource_ready != 0) {
                state->layer.flags.bits.resource_ready = 0;
                func_ov002_020725a4(
                    func_ov002_02093b88, &state->layer, 0, 0);
                state->displayed_side = state->requested_side;
            }
        }
    } else if (state->layer.flags.bits.resource_ready != 0) {
        state->layer.flags.bits.resource_ready = 0;
        func_ov002_020725a4(
            func_ov002_02093b88, &state->layer, 0, 0);
    } else if (state->transition_angle > 0) {
        state->transition_angle -= 12;
    }

    if (state->transition_angle < 0) {
        state->transition_angle = 0;
    }
}

void BattleTargetLabel_Draw(BattleTargetLabelState *state) {
    BattlePosition position;
    BattleTargetLabelTransform transform;
    BattleSceneObject *object;
    int sine_index;

    if (state->displayed_side == 0 || state->intensity <= 0) {
        return;
    }

    object = state->scene_object;
    BattlePosition_StoreViewRelative(
        &position,
        object->x,
        (s16)(object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model
    );

    sine_index =
        2 * ((int)(u16)(state->transition_angle << 8) >> 4) + 1;
    transform = gBattleTargetLabelTransformTemplate;
    transform.value.matrix[5] = FX_SinCosTable_[sine_index];
    transform.value.x = position.x << 8;
    transform.value.y = position.y << 8;

    if (state->displayed_side == 2) {
        transform.value.matrix[0] = -transform.value.matrix[0];
        transform.value.x = (position.x << 8) - 30720;
        BattleSprite_DrawFrame(
            19, 31, &transform.value, 0, 0x20, 3, 0x7FFF);
        transform.value.matrix[0] = -transform.value.matrix[0];
        transform.value.scale--;
        *(volatile u32 *)0x04000440 = 2;
        func_02036cc0(&transform.value);
        func_ov002_020925bc(&transform.value, &state->layer);
        return;
    } else {
        BattleSprite_DrawFrame(
            19, 31, &transform.value, 0, 0x20, 3, 0x7FFF);
        func_ov002_020925bc(&transform.value, &state->layer);
        return;
    }
}

void BattleTargetMarker_Update(BattleTargetOverlayState *state) {
    BattleSceneObject *object = state->primary_marker;
    BattleModel *model;
    int animation_id;

    if (object == 0) {
        return;
    }

    animation_id = state->animation_id;
    model = object->primary_model;
    if (animation_id < 0) {
        state->intensity -= 2;
    } else {
        if (state->intensity < 31) {
            state->intensity += 4;
        }
        if (animation_id != model->animation_id) {
            model->set_primary_animation((u8)animation_id, 0, 1);
        }
    }

    if (state->intensity <= 0) {
        state->intensity = 0;
        return;
    }
    if (state->intensity < 31) {
        BattleModel_SetAlpha(model, (u8)state->intensity, 0);
        return;
    }
    if (state->intensity <= 31) {
        return;
    }

    state->intensity = 31;
    BattleModel_SetAlpha(model, (u8)state->intensity, 0);
}

void BattleTargetOverlay_Draw(BattleTargetOverlayState *state) {
    BattleSceneObject *primary_object;
    BattleModel *model;
    BattleSpriteTransform *transform;
    BattlePosition position;
    u8 *context;
    int actor_index;
    int active_actor_id;

    if (state->intensity < 1) {
        return;
    }

    primary_object = state->primary_marker;
    if (*(s16 *)(gBattleContext + BATTLE_GLOBAL_01_OFFSET) == 3 &&
        state->animation_id >= 0 &&
        state->target_group == BATTLE_TARGET_GROUP_ENEMIES &&
        ((BattleTargetInterfaceFlags *)(gBattleContext +
          BATTLE_TARGET_INTERFACE_FLAGS_OFFSET))->bits.uniform_markers == 0 &&
        ((BattleTargetInputFlags *)(
             *(u8 **)(gBattleContext + BATTLE_TARGET_INPUT_STATE_OFFSET) +
             BATTLE_TARGET_INPUT_FLAGS_OFFSET))->bits.confirm_held != 0) {
        model = primary_object->primary_model;
        transform = model->get_sprite_transform();
        model->prepare_render();
        for (actor_index = 0;
             actor_index < BATTLE_ACTOR_ENEMY_COUNT - 2;
             actor_index++) {
            u16 actor_id = actor_index + BATTLE_ACTOR_ENEMY_FIRST;
            BattleActor *actor = BattleActor_GetById((u16)actor_id);

            if (BattleActor_CanReceiveStatus(actor) &&
                BattleActor_GetById((u16)actor_id)->
                    flag_bits.excluded_from_targeting == 0) {
                BattleSceneObject *object =
                    BattleSceneObject_GetById((u16)actor_id);
                s16 anchor_y = object->y + object->property_0fd;
                s16 anchor_offset = object->z + object->property_0f8;

                BattlePosition_StoreViewRelative(
                    &position,
                    (s16)(object->x + object->property_0fc),
                    (s16)(anchor_y - anchor_offset),
                    (s16)(object->effect_anchor_z +
                          16 * (256 - anchor_y)),
                    object->flags.bits.use_raw_position,
                    object->flags.bits.use_alternate_model
                );
                transform->x = position.x << 8;
                transform->y = position.y << 8;
                transform->scale = 0;
                model->draw(0, 0, 0);
                transform->matrix[0] = 4096;
            }
        }

        model = state->secondary_marker->primary_model;
        transform = model->get_sprite_transform();
        {
            BattleSceneObject *focus_object = *(BattleSceneObject **)(
                gBattleContext + BATTLE_TARGET_FOCUS_OBJECT_OFFSET);
            s16 focus_y = focus_object->y + 8;

            BattlePosition_StoreViewRelative(
                &position,
                (s16)(focus_object->x - 8),
                (s16)(focus_y - focus_object->z),
                (s16)(focus_object->effect_anchor_z +
                      16 * (256 - focus_y)),
                focus_object->flags.bits.use_raw_position,
                focus_object->flags.bits.use_alternate_model
            );
        }
        transform->matrix[5] = -4096;
        transform->x = position.x << 8;
        transform->y = position.y << 8;
        transform->scale = 0;
        model->prepare_render();
        model->draw(0, 0, 0);
        transform->matrix[5] = 4096;
        return;
    }

    if (primary_object != 0) {
        model = primary_object->primary_model;
        transform = model->get_sprite_transform();
        BattlePosition_StoreViewRelative(
            &position,
            primary_object->x,
            (s16)(primary_object->y - primary_object->z),
            (s16)(primary_object->effect_anchor_z +
                  16 * (256 - primary_object->y)),
            primary_object->flags.bits.use_raw_position,
            primary_object->flags.bits.use_alternate_model
        );
        transform->x = position.x << 8;
        transform->y = position.y << 8;
        transform->scale = 0;
        model->prepare_render();
        model->draw(0, 0, 0);
        transform->matrix[0] = 4096;
    }

    if (state->animation_id < 0) {
        return;
    }

    context = gBattleContext;
    active_actor_id = *(s16 *)(context + BATTLE_GLOBAL_02_OFFSET);
    model = state->secondary_marker->primary_model;
    if (model == 0) {
        return;
    }

    model->prepare_render();
    transform = model->get_sprite_transform();
    switch (state->target_group) {
    case BATTLE_TARGET_GROUP_ENEMIES:
        if (((BattleTargetInterfaceFlags *)(gBattleContext +
             BATTLE_TARGET_INTERFACE_FLAGS_OFFSET))->bits.uniform_markers ==
            0) {
            BattleSceneObject *focus_object = *(BattleSceneObject **)(
                gBattleContext + BATTLE_TARGET_FOCUS_OBJECT_OFFSET);
            s16 focus_y = focus_object->y + 8;

            BattlePosition_StoreViewRelative(
                &position,
                (s16)(focus_object->x - 8),
                (s16)(focus_y - focus_object->z),
                (s16)(focus_object->effect_anchor_z +
                      16 * (256 - focus_y)),
                focus_object->flags.bits.use_raw_position,
                focus_object->flags.bits.use_alternate_model
            );
            transform->matrix[5] = -4096;
            transform->x = position.x << 8;
            transform->y = position.y << 8;
            transform->scale = 0;
            model->draw(0, 0, 0);
            transform->matrix[5] = 4096;
        }

        for (actor_index = 0;
             actor_index < BATTLE_ACTOR_ENEMY_COUNT - 2;
             actor_index++) {
            BattleActor *actor;

            if (active_actor_id ==
                    actor_index + BATTLE_ACTOR_ENEMY_FIRST &&
                ((BattleTargetInterfaceFlags *)(gBattleContext +
                  BATTLE_TARGET_INTERFACE_FLAGS_OFFSET))->bits.
                    uniform_markers == 0) {
                continue;
            }

            actor = BattleActor_GetEnemySlot(
                (u16)(actor_index + BATTLE_ACTOR_ENEMY_FIRST));
            if (actor->resource_slot != 0 &&
                actor->current_hp > 0 &&
                actor->flag_bits.excluded_from_targeting == 0) {
                BattleSceneObject *object = actor->scene_object;
                s16 anchor_y = object->y + object->property_0fd;
                s16 anchor_offset = object->z + object->property_0f8;

                BattlePosition_StoreViewRelative(
                    &position,
                    (s16)(object->x + object->property_0fc),
                    (s16)(anchor_y - anchor_offset),
                    (s16)(object->effect_anchor_z +
                          16 * (256 - anchor_y)),
                    object->flags.bits.use_raw_position,
                    object->flags.bits.use_alternate_model
                );
                transform->x = position.x << 8;
                transform->y = position.y << 8;
                transform->scale = 16;
                model->draw(0, 0, 0);
            }
        }
        return;

    case BATTLE_TARGET_GROUP_PARTY:
        if (((BattleTargetInterfaceFlags *)(gBattleContext +
             BATTLE_TARGET_INTERFACE_FLAGS_OFFSET))->bits.uniform_markers ==
            0) {
            BattleSceneObject *focus_object = *(BattleSceneObject **)(
                gBattleContext + BATTLE_TARGET_FOCUS_OBJECT_OFFSET);
            s16 focus_y = focus_object->y + 8;

            BattlePosition_StoreViewRelative(
                &position,
                (s16)(focus_object->x - 8),
                (s16)(focus_y - focus_object->z),
                (s16)(focus_object->effect_anchor_z +
                      16 * (256 - focus_y)),
                focus_object->flags.bits.use_raw_position,
                focus_object->flags.bits.use_alternate_model
            );
            transform->matrix[0] = -4096;
            transform->matrix[5] = -4096;
            transform->x = 10240;
            transform->y = position.y << 8;
            transform->scale = 0;
            model->draw(0, 0, 0);
            transform->matrix[0] = 4096;
            transform->matrix[5] = 4096;
        }

        for (actor_index = 0;
             actor_index < BATTLE_ACTOR_PARTY_COUNT;
             actor_index++) {
            if (active_actor_id ==
                    actor_index + BATTLE_ACTOR_PARTY_FIRST &&
                ((BattleTargetInterfaceFlags *)(gBattleContext +
                  BATTLE_TARGET_INTERFACE_FLAGS_OFFSET))->bits.
                    uniform_markers == 0) {
                continue;
            }
            if (BattleActor_GetPartySlot(
                    (u16)(actor_index + BATTLE_ACTOR_PARTY_FIRST))->
                    flag_bits.excluded_from_targeting == 0) {
                BattleSceneObject *object = BattleSceneObject_GetById(
                    (u16)((actor_index & 1) + BATTLE_ACTOR_PARTY_FIRST));
                s16 anchor_y = object->y + object->property_0fd;
                s16 anchor_offset = object->z + object->property_0f8;

                BattlePosition_StoreViewRelative(
                    &position,
                    (s16)(object->x + object->property_0fc),
                    (s16)(anchor_y - anchor_offset),
                    (s16)(object->effect_anchor_z +
                          16 * (256 - anchor_y)),
                    object->flags.bits.use_raw_position,
                    object->flags.bits.use_alternate_model
                );
                if (actor_index < 2) {
                    transform->matrix[0] = -4096;
                } else {
                    position.x -= 16;
                }
                transform->x = position.x << 8;
                transform->y = position.y << 8;
                transform->scale = 0;
                model->draw(0, 0, 0);
                transform->matrix[0] = 4096;
            }
        }
        return;
    }
}

void BattlePartyIndicator_Update(BattlePartyIndicatorState *state) {
    BattleSceneObject *object = state->object;
    BattlePartyActor *actor;

    if (object == 0) {
        return;
    }
    if (state->actor_id < BATTLE_ACTOR_PARTY_FIRST) {
        return;
    }

    actor = (BattlePartyActor *)BattleActor_GetPartySlot(state->actor_id);
    if (state->enabled == 0 ||
        *(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET) == -1 ||
        actor->actor.current_hp <= 0 ||
        actor->actor.transition_state > 0 ||
        actor->state_flags.bits.flag_12 != 0) {
        if (state->alpha == 0) {
            return;
        }
        if (state->alpha > 0) {
            state->alpha -= 2;
            if (state->alpha <= 0) {
                state->alpha = 0;
                BattleSceneObject_SetAnimation(object, -1, -1);
                return;
            }
        }
    } else if (state->alpha < 31) {
        if (state->alpha == 0) {
            state->previous_party_group = (u16)-2;
        }
        state->alpha = 31;
    }

    {
        int party_group =
            *(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET);

        if (state->previous_party_group != party_group &&
            party_group != -1) {
            int animation = 0;

            state->previous_party_group = party_group;
            if (state->previous_party_group == 0) {
                BattleModelEffect_Spawn(46, object, 0, 0, 0, 256);
                switch (((BattlePartyActor *)BattleActor_GetPartySlot(
                             state->actor_id))->formation_index) {
                case 0:
                case 4:
                    animation = BATTLE_PARTY_INDICATOR_ADULT_FRONT;
                    break;
                case 2:
                    animation = BATTLE_PARTY_INDICATOR_BABY_FRONT;
                    break;
                case 1:
                case 5:
                    animation = BATTLE_PARTY_INDICATOR_ADULT_BACK;
                    break;
                case 3:
                    animation = BATTLE_PARTY_INDICATOR_BABY_BACK;
                    break;
                }
            }
            if (state->previous_party_group == 1) {
                BattleModelEffect_Spawn(46, object, 0, 0, 0, 256);
                switch (((BattlePartyActor *)BattleActor_GetPartySlot(
                             state->actor_id))->formation_index) {
                case 0:
                case 2:
                case 4:
                    animation = BATTLE_PARTY_INDICATOR_BABY_GROUP_A;
                    break;
                case 1:
                case 3:
                case 5:
                    animation = BATTLE_PARTY_INDICATOR_BABY_GROUP_B;
                    break;
                }
            }
            BattleSceneObject_SetAnimation(object, animation, -1);
        }
    }

    BattleModel_SetAlpha(object->primary_model, (u8)state->alpha, 0);
}

}
