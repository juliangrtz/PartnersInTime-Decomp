#include <game/battle_scene.h>
#include <game/battle_actor.h>
#include <game/battle_context.h>

extern "C" {
extern void OS_Terminate(void);
extern void *func_02029be0(u32 size, u32 heap_id, const void *allocator,
                          int argument_3);
extern BattleModel *func_02010634(BattleModel *model);
extern void func_ov002_020696f4(BattleSceneObject *object, int animation_id,
                                int argument_2);
extern void func_ov002_0206f1b8(BattleModel *model);
extern void func_ov002_0206f1f0(BattleModel *model,
                                BattleSceneResource *resource,
                                u32 component_offset);
extern void func_ov002_0206f384(BattleModel *model);
extern const char data_ov002_020bf718[];
}

enum {
    BATTLE_SCENE_OBJECT_ID_LIMIT = 70,
    BATTLE_ANIMATION_OBSERVER_OFFSET = 0x68C8
};

enum BattleSceneActorKind {
    BATTLE_SCENE_ACTOR_OTHER,
    BATTLE_SCENE_ACTOR_PARTY,
    BATTLE_SCENE_ACTOR_ENEMY
};

typedef struct BattleAnimationObserver BattleAnimationObserver;

typedef struct BattleAnimationObserverVTable {
    u8 unknown_00[0x90];
    void (*refresh_model)(BattleAnimationObserver *observer,
                          BattleModel *model);
} BattleAnimationObserverVTable;

struct BattleAnimationObserver {
    BattleAnimationObserverVTable *vtable;
};

#define BATTLE_PARTY_ANIMATION_RESOURCE_CASES \
    case 0xC0000000u:                         \
    case 0xC0000001u:                         \
    case 0xC0000002u:                         \
    case 0xC0000003u:                         \
    case 0xC000000Bu:                         \
    case 0xC000000Du:                         \
    case 0xC0000053u:                         \
    case 0xC0000054u:                         \
    case 0xC0000055u:                         \
    case 0xC0000056u:                         \
    case 0xC0000057u:                         \
    case 0xC0000058u

int BattleSceneObject_IsAnimationActiveById(u32 object_id,
                                            int channel_index) {
    if (object_id >= BATTLE_SCENE_OBJECT_ID_LIMIT) {
        OS_Terminate();
    }
    return BattleSceneObject_IsAnimationChannelActive(
        BattleSceneObject_GetById(object_id), channel_index);
}

int BattleSceneObject_IsAnimationChannelActive(BattleSceneObject *object,
                                               int channel_index) {
    if (channel_index >= BATTLE_MOTION_CHANNEL_COUNT) {
        OS_Terminate();
    }
    if (object->motion_channels[channel_index].callback == 0) {
        return 0;
    }
    return 1;
}

void BattleSceneObject_SetAnimation(BattleSceneObject *object,
                                    int animation_id, int argument_2) {
    BattleModel *model;
    u32 actor_id;
    BattlePartyActor *party_actor;
    int refresh_model;
    int actor_kind;

    if (object->resource == 0) {
        OS_Terminate();
    }
    if (object->resource->flags.bits.processing) {
        OS_Terminate();
    }

    actor_id = object->actor_id;
    model = object->primary_model;
    actor_kind = BATTLE_SCENE_ACTOR_OTHER;
    if (actor_id >= BATTLE_ACTOR_PARTY_FIRST &&
        actor_id < BATTLE_ACTOR_PARTY_FIRST + BATTLE_ACTOR_PARTY_COUNT) {
        actor_kind = BATTLE_SCENE_ACTOR_PARTY;
    } else if (actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
               actor_id < BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
        actor_kind = BATTLE_SCENE_ACTOR_ENEMY;
    }

    if (object->flags.bits.script_mode != 0) {
        if (model == 0) {
            model = (BattleModel *)func_02029be0(
                sizeof(BattleModel),
                ((BattleContext *)gBattleContext)->runtime.resource_heap_id,
                data_ov002_020bf718, 0);
            if (model != 0) {
                model = func_02010634(model);
            }
            object->primary_model = model;
            model->unk_010[0] = 0;
            model->start();
            model->render_flags &= ~4;
            model->transform_flags &= ~0x20;
            model->flags |= BATTLE_MODEL_FLAG_ANIMATION_ACTIVE;
            model->animation_offset_x = 0;
            model->animation_offset_y = 0;
            model->scale_x = 0x100;
            model->scale_y = 0x100;
            model->owner = object;
        }
        if (animation_id < 0) {
            model->stop();
            model->flags &= ~BATTLE_MODEL_FLAG_ANIMATION_ACTIVE;
        } else {
            model->start();
            model->flags |= BATTLE_MODEL_FLAG_ANIMATION_ACTIVE;
        }
        return;
    }

    object->flags.bits.state = 0;
    if (argument_2 == 0 ||
        (argument_2 != 1 && !object->flags.bits.use_alternate_model)) {
        if (actor_kind == BATTLE_SCENE_ACTOR_ENEMY) {
            BattleActor *enemy = BattleActor_GetEnemySlot(actor_id);

            if (animation_id == 0 && enemy->transition_state == 2) {
                animation_id = *(s16 *)((u8 *)enemy + 0x29A);
            }
        } else if (actor_kind == BATTLE_SCENE_ACTOR_PARTY &&
                   object->animation_variant_offset == 0) {
            u32 resource_id;

            party_actor =
                (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
            if (animation_id == 0) {
                resource_id = object->resource->resource_id;
                if (resource_id == (u32)object->loaded_resource_id) {
                    switch (resource_id) {
                        BATTLE_PARTY_ANIMATION_RESOURCE_CASES:
                            if (model != 0 &&
                                model->flag_bits.animation_active &&
                                model->animation_id == animation_id) {
                                return;
                            }
                            break;
                        default:
                            break;
                    }
                }

                switch (resource_id) {
                    BATTLE_PARTY_ANIMATION_RESOURCE_CASES:
                        if (party_actor->actor.transition_state == 1) {
                            animation_id = 25;
                        } else if (party_actor->actor.transition_state == 2) {
                            animation_id = 27;
                        } else if (party_actor->actor.transition_state == 3) {
                            animation_id = 29;
                        } else if (party_actor->actor.force_low_hp_animation) {
                            animation_id = 22;
                        } else if (BattleActor_IsHpAtMostQuarter(
                                       BattleActor_GetById(actor_id))) {
                            animation_id = 22;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (animation_id >= 0 &&
                party_actor->actor.transition_state == 3 &&
                party_actor->actor.animation_state != 0) {
                BattleModel *transition_model =
                    party_actor->actor.animation_state->model;
                int transition_animation_id;

                switch ((u32)object->resource->resource_id) {
                    BATTLE_PARTY_ANIMATION_RESOURCE_CASES:
                        transition_animation_id = animation_id;
                        switch (party_actor->formation_index) {
                            case 1:
                                transition_animation_id = animation_id + 44;
                                break;
                            case 2:
                                transition_animation_id = animation_id + 88;
                                break;
                            case 3:
                                transition_animation_id = animation_id + 132;
                                break;
                            case 4:
                                transition_animation_id = animation_id + 176;
                                break;
                            case 5:
                                transition_animation_id = animation_id + 220;
                                break;
                            default:
                                break;
                        }
                        transition_model->set_animation(
                            (u8)transition_animation_id, 0);
                        transition_model->flags &=
                            ~BATTLE_MODEL_ANIMATION_MODE_MASK;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    if (animation_id >= 0) {
        animation_id += object->animation_variant_offset;
        object->previous_base_animation_id =
            object->animation_id - object->animation_variant_offset;
        object->animation_id = animation_id;
    }

    /* Determine whether the battle renderer must be told about a newly
     * activated model after this animation change. */
    {
        refresh_model = 0;

        if (animation_id >= 0) {
            BattleAnimationObserver *observer =
                *(BattleAnimationObserver **)(gBattleContext +
                                              BATTLE_ANIMATION_OBSERVER_OFFSET);

            if (observer != 0 &&
                ((u32)object->resource->resource_id & 0xFF000000) ==
                    0xC0000000u &&
                object->resource->texture_set->variant_index == 0) {
                if (object->loaded_resource_id !=
                    object->resource->resource_id) {
                    refresh_model = 1;
                } else if (argument_2 == 0 ||
                           (argument_2 != 1 &&
                            !object->flags.bits.use_alternate_model)) {
                    if (object->primary_model == 0 ||
                        !object->primary_model->flag_bits.animation_active) {
                        refresh_model = 1;
                    }
                } else {
                    if (object->alternate_model == 0 ||
                        !object->alternate_model->flag_bits.animation_active) {
                        refresh_model = 1;
                    }
                }
            }
        }

        if (argument_2 == 0) {
            if (object->flags.bits.use_alternate_model) {
                object->flags.bits.use_alternate_model = 0;
                if (object->alternate_model != 0) {
                    func_ov002_0206f384(object->alternate_model);
                }
            }
        } else {
            func_ov002_020696f4(object, animation_id, argument_2);
        }

        if (!object->flags.bits.use_alternate_model) {
            if (animation_id < 0) {
                object->loaded_resource_id = object->resource->resource_id;
                if (model != 0 && model->flag_bits.animation_active) {
                    func_ov002_0206f1b8(model);
                    model->flags &= ~BATTLE_MODEL_FLAG_ANIMATION_ACTIVE;
                }
                return;
            }

            /* Keep these locals scoped to the primary-model path; their
             * original lifetimes affect the compiler's register allocation. */
            {
                int had_model = 0;
                int saved_animation_state;
                u32 resource_id;

                if (model != 0) {
                    had_model = 1;
                    saved_animation_state =
                        model->animation_state_bits.state;
                }

                resource_id = object->resource->resource_id;
                if ((u32)object->loaded_resource_id != resource_id) {
                    object->loaded_resource_id = resource_id;
                    if (model != 0) {
                        func_ov002_0206f1b8(model);
                        model->flags &= ~BATTLE_MODEL_FLAG_ANIMATION_ACTIVE;
                    }
                }

                if (model == 0) {
                    model = (BattleModel *)func_02029be0(
                        sizeof(BattleModel),
                        ((BattleContext *)gBattleContext)
                            ->runtime.resource_heap_id,
                        data_ov002_020bf718, 0);
                    if (model != 0) {
                        model = func_02010634(model);
                    }
                    object->primary_model = model;
                    model->flags &= ~BATTLE_MODEL_FLAG_ANIMATION_ACTIVE;
                    model->owner = object;
                }

                model->flags &= ~BATTLE_MODEL_FLAG_09;
                model->flags &= ~BATTLE_MODEL_FLAG_02;
                if (!model->flag_bits.animation_active) {
                    func_ov002_0206f1f0(
                        model, object->resource,
                        (object->resource->flags.bits.resource_index - 1) <<
                            6);
                    model->animation_offset_x = 0;
                    model->animation_offset_y = 0;
                }
                model->set_primary_animation((u8)animation_id, 0, 1);

                if (had_model) {
                    model->animation_state_bits.state =
                        saved_animation_state;
                }

                switch (actor_kind) {
                    case BATTLE_SCENE_ACTOR_ENEMY: {
                        BattleActor *enemy = BattleActor_GetById(actor_id);

                        if (animation_id == 0 &&
                            enemy->transition_state == 3) {
                            model->enemy_idle_frame = 0x200;
                        }
                        break;
                    }
                    case BATTLE_SCENE_ACTOR_PARTY:
                    default:
                        break;
                }
            }
        }

        if (refresh_model) {
            BattleModel *active_model =
                BattleSceneObject_GetActiveModel(object);
            BattleAnimationObserver *observer =
                *(BattleAnimationObserver **)(
                    gBattleContext + BATTLE_ANIMATION_OBSERVER_OFFSET);
            observer->vtable->refresh_model(observer, active_model);
        }
    }
}

u32 BattleSceneObject_SetAnimationFromComponent(BattleSceneObject *object,
                                                int animation_id,
                                                int argument_2,
                                                int component_index) {
    u32 saved_resource_index;
    u32 result;

    if (object->resource == 0) {
        OS_Terminate();
    }
    saved_resource_index = object->resource->flags.bits.resource_index;
    object->resource->flags.bits.resource_index = component_index + 1;
    BattleSceneObject_SetAnimation(object, animation_id, argument_2);
    result = (object->resource->flags.raw & 0xF8000000) |
             (saved_resource_index & 0x07FFFFFF);
    object->resource->flags.raw = result;
    return result;
}
