#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>

extern "C" {
extern void DC_FlushRange(void *start, u32 size);
extern void func_01ff861c(int channel, void *source, u32 size);
}

enum BattleShadowConstant {
    BATTLE_SHADOW_RESOURCE_ID = 2,
    BATTLE_SHADOW_RENDER_BUFFER_OFFSET = 0x68D8,
    BATTLE_SHADOW_RENDER_BUFFER_START_OFFSET = 0x15568,
    BATTLE_SHADOW_POLYGON_ID_OFFSET = 0x68D2,
    BATTLE_SHADOW_POLYGON_ID_LIMIT = 63,
    BATTLE_SHADOW_POLYGON_ID_FIRST = 53,
    BATTLE_SHADOW_GROUND_ANCHOR_OFFSET = 0x40,
    BATTLE_SHADOW_VIEW_Y_OFFSET = 0xCB9E,
    BATTLE_SHADOW_OPACITY_MAX = 63,
    BATTLE_SHADOW_ATTRIBUTE_MASK = (s32)0xC0E0FFFF,
    BATTLE_SHADOW_DISPLAY_LIST_CHANNEL = 1
};

typedef struct BattleShadowDisplayList {
    u8 unknown_00[0x0C];
    s32 scale_x;
    u8 unknown_10[0x10];
    s32 scale_y;
    u8 unknown_24[0x18];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_48[4];
    u32 attributes;
} BattleShadowDisplayList;

typedef char BattleShadowDisplayList_SizeCheck[
    sizeof(BattleShadowDisplayList) == 0x50 ? 1 : -1];

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
extern "C" int BattleScene_UpdateShadowVisibility(
    BattleSceneObject *object) {
    BattleModel *model = BattleSceneObject_GetActiveModel(object);

    if (((model->flags << 29) >> 31) != 0 &&
        object->flags.shadow_delay > 0) {
        object->flags.shadow_delay--;
        if (object->flags.shadow_delay == 0) {
            object->flags.shadow_delay = -1;
            if (object->flags.bits.independent_flag) {
                **(u16 **)(gBattleContext +
                           BATTLE_SHADOW_RENDER_BUFFER_OFFSET) =
                    object->actor_id;
                *(u16 **)(gBattleContext +
                           BATTLE_SHADOW_RENDER_BUFFER_OFFSET) += 1;
                return 0;
            }
            model->flags |= BATTLE_MODEL_FLAG_09;
        }
    }
    return 1;
}

extern "C" void BattleScene_HideQueuedObjects(void) {
    int hidden_animation = -1;
    int end_address = *(int *)(gBattleContext +
                               BATTLE_SHADOW_RENDER_BUFFER_OFFSET);
    u16 *current;

    current = (u16 *)(gBattleContext +
                      BATTLE_SHADOW_RENDER_BUFFER_START_OFFSET);

    while ((int)current < end_address) {
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(*current),
            hidden_animation,
            hidden_animation);
        current++;
    }
}

extern "C" void BattleScene_DrawShadow(int variant, int animation_state,
                                        int x, int y, int height, int alpha) {
    int template_index = variant;
    BattleSceneResource *resource;
    BattleShadowDisplayList *display_list;
    u8 **templates;
    u32 display_list_size;
    int scale;

    if (height < 0 || animation_state <= 0) {
        return;
    }

    animation_state = 24 * animation_state / (height + 48) + 1;
    resource = BattleObjectData_GetLoadState(BATTLE_SHADOW_RESOURCE_ID);
    templates = (u8 **)resource->data;
    display_list = (BattleShadowDisplayList *)templates[template_index];
    display_list_size =
        templates[template_index + 1] - templates[template_index];

    (*(u16 *)(gBattleContext + BATTLE_SHADOW_POLYGON_ID_OFFSET))++;
    if (*(u16 *)(gBattleContext + BATTLE_SHADOW_POLYGON_ID_OFFSET) >=
        BATTLE_SHADOW_POLYGON_ID_LIMIT) {
        *(u16 *)(gBattleContext + BATTLE_SHADOW_POLYGON_ID_OFFSET) =
            BATTLE_SHADOW_POLYGON_ID_FIRST;
    }

    scale = 409600 / (height + 99);
    display_list->scale_x = scale;
    display_list->scale_y = scale;
    display_list->x = x << 8;
    display_list->y = y << 8;
    display_list->z =
        *(s16 *)(gBattleContext + BATTLE_SHADOW_GROUND_ANCHOR_OFFSET) + 64 +
        16 * (256 -
              (y + *(s16 *)(gBattleContext + BATTLE_SHADOW_VIEW_Y_OFFSET)));
    *(u16 *)(gBattleContext + BATTLE_SHADOW_GROUND_ANCHOR_OFFSET) = 0;
    display_list->attributes =
        (display_list->attributes & BATTLE_SHADOW_ATTRIBUTE_MASK) |
        (*(u16 *)(gBattleContext + BATTLE_SHADOW_POLYGON_ID_OFFSET) << 24) |
        (animation_state << 16);
    DC_FlushRange(display_list, display_list_size);
    func_01ff861c(BATTLE_SHADOW_DISPLAY_LIST_CHANNEL,
                  display_list, display_list_size);

    if (height > 0) {
        int second_alpha = animation_state * alpha / 128;

        if (second_alpha == 0 && alpha != 0) {
            if ((*(u32 *)(gBattleContext + 0x14) & 1) != 0) {
                second_alpha = 1;
            }
        }
        if (second_alpha > 0) {
            scale = (height << 8) + 4096;
            display_list->scale_x = scale;
            display_list->scale_y = scale;
            display_list->y = (y + 3 * scale / 8192) << 8;
            display_list->attributes =
                (display_list->attributes & BATTLE_SHADOW_ATTRIBUTE_MASK) |
                (*(u16 *)(gBattleContext +
                           BATTLE_SHADOW_POLYGON_ID_OFFSET) << 24) |
                (second_alpha << 16);
            DC_FlushRange(display_list, display_list_size);
            func_01ff861c(BATTLE_SHADOW_DISPLAY_LIST_CHANNEL,
                          display_list, display_list_size);
        }
    }
}

static inline BattleModel *BattleScene_GetSelectedModel(
    BattleSceneObject *object, int selector) {
    if (selector == 0) {
        return object->primary_model;
    }
    if (selector == 1) {
        return object->alternate_model;
    }
    return BattleSceneObject_GetActiveModel(object);
}

extern "C" int BattleScene_TestModelRelation(BattleSceneObject *first,
                                               BattleSceneObject *second,
                                               int first_model,
                                               int second_model) {
    BattleModel *reference =
        BattleScene_GetSelectedModel(first, first_model);
    BattleModel *target =
        BattleScene_GetSelectedModel(second, second_model);

    return target->test_relation(reference);
}

extern "C" void BattleScene_UpdateAlternateModelAnchor(BattleModel *model) {
    BattleSceneObject *object = model->owner;

    if (object == 0) {
        return;
    }
    if (object->alternate_model != model) {
        return;
    }

    BattlePosition position;
    s16 offset_x;
    s16 offset_y;

    BattlePosition_StoreViewRelative(
        &position,
        object->x,
        (s16)(object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);
    offset_x = position.x + object->property_102;
    offset_y = position.y + object->property_103;
    model->animation_offset_x = offset_x;
    model->animation_offset_y = offset_y;
    model->render_anchor_z = position.z;
    BattleScene_UpdateShadowVisibility(object);
}

extern "C" void BattleScene_RenderShadows(void) {
    BattleModel *model;

    *(u8 **)(gBattleContext + BATTLE_SHADOW_RENDER_BUFFER_OFFSET) =
        gBattleContext + BATTLE_SHADOW_RENDER_BUFFER_START_OFFSET;
    for (model = gModelRenderList; model != 0; model = model->render_next) {
        BattleSceneObject *object = model->owner;

        if (object->flags.bits.render_mode != 1) {
            int visible = BattleScene_UpdateShadowVisibility(object);

            if (model->flag_bits.animation_active && visible &&
                !object->flags.bits.suppress_shadow) {
                if (object->render_override != 0 &&
                    object->render_override->render != 0 &&
                    object->render_override->render(object, 1)) {
                    continue;
                }
                {
                    BattlePosition position;
                    int variant;
                    int alpha;
                    int height;

                    BattlePosition_StoreViewRelative(
                        &position,
                        object->x,
                        (s16)(object->y - object->z),
                        (s16)(object->effect_anchor_z +
                              16 * (256 - object->y)),
                        object->flags.bits.use_raw_position,
                        object->flags.bits.use_alternate_model);
                    int override = (object->flags.raw << 8) >> 28;

                    if (override != 0) {
                        variant = override - 1;
                    } else if (object->resource->texture_set != 0) {
                        variant = object->resource->texture_set->shadow_variant;
                    } else {
                        continue;
                    }

                    alpha = object->flags.bits.shadow_alpha;
                    height = object->z;
                    int motion_origin = object->motion_origin_z;

                    if (height == 0) {
                        alpha = BATTLE_SHADOW_OPACITY_MAX;
                    } else if (height == motion_origin) {
                        if (alpha > 0) {
                            alpha--;
                        }
                    } else {
                        int height_alpha = 29696 / (height * height + 512);

                        alpha += 3;
                        if (height_alpha < alpha) {
                            alpha = height_alpha;
                        }
                    }
                    object->flags.bits.shadow_alpha = alpha;
                    *(u16 *)(gBattleContext +
                             BATTLE_SHADOW_GROUND_ANCHOR_OFFSET) =
                        object->effect_anchor_z;
                    BattleScene_DrawShadow(
                        variant,
                        model->animation_state_bits.state,
                        position.x,
                        position.y + height,
                        height,
                        alpha);
                }
            }
        }
    }
    BattleScene_HideQueuedObjects();
}
