#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

extern "C" {
extern const s16 FX_SinCosTable_[];
extern void MTX_Identity44_(s32 *matrix);
extern void MTX_ApplyRotZ44(int cosine, int sine,
                            s32 *x_column, s32 *y_column);
extern void func_ov002_020ad880(BattleSpriteTransform *transform,
                                int animation_state);
extern void func_ov002_020bc670(BattleSceneObject *object);
extern int func_ov002_0206a02c(void);
}

enum BattleSceneRenderConstant {
    BATTLE_RENDER_OBSERVER_OFFSET = 0x68C8,
    BATTLE_RENDER_FRAME_OFFSET = 0x68D4,
    BATTLE_RENDER_BUFFER_OFFSET = 0x68D8,
    BATTLE_RENDER_BUFFER_START_OFFSET = 0x15568,
    BATTLE_RENDER_SCALE_ONE = 256,
    BATTLE_RENDER_MATRIX_ONE = 4096,
    BATTLE_RENDER_FRAME_LIMIT = 18,
    BATTLE_RENDER_FRAME_FIRST = 8,
    BATTLE_RENDER_STATE_NONE = 0,
    BATTLE_RENDER_STATE_END = 31
};

struct BattleRenderObserver {
    virtual void begin_frame();
};

extern "C" int BattleScene_RenderModels(void) {
    BattleRenderObserver *observer = *(BattleRenderObserver **)(
        gBattleContext + BATTLE_RENDER_OBSERVER_OFFSET);
    BattleModel *model;

    if (observer != 0) {
        observer->begin_frame();
    }

    for (model = gModelRenderList; model != 0; model = model->render_next) {
        BattleSceneObject *object = model->owner;

        if (model->flag_bits.animation_active) {
            BattleSpriteTransform *transform =
                model->get_sprite_transform();
            int scale;

            if (model->flag_bits.facing_left) {
                transform->matrix[0] = -transform->matrix[0];
                transform->matrix[4] = -transform->matrix[4];
                transform->matrix[8] = -transform->matrix[8];
                transform->matrix[12] = -transform->matrix[12];
            }
            if (model->flag_bits.flip_y) {
                transform->matrix[1] = -transform->matrix[1];
                transform->matrix[5] = -transform->matrix[5];
                transform->matrix[9] = -transform->matrix[9];
                transform->matrix[13] = -transform->matrix[13];
            }

            scale = model->scale_x;
            if (scale != BATTLE_RENDER_SCALE_ONE) {
                transform->matrix[0] =
                    transform->matrix[0] * scale / BATTLE_RENDER_SCALE_ONE;
                transform->matrix[4] =
                    transform->matrix[4] * scale / BATTLE_RENDER_SCALE_ONE;
                transform->matrix[8] =
                    transform->matrix[8] * scale / BATTLE_RENDER_SCALE_ONE;
                transform->matrix[12] =
                    transform->matrix[12] * scale / BATTLE_RENDER_SCALE_ONE;
            }

            scale = model->scale_y;
            if (scale != BATTLE_RENDER_SCALE_ONE) {
                transform->matrix[1] =
                    transform->matrix[1] * scale / BATTLE_RENDER_SCALE_ONE;
                transform->matrix[5] =
                    transform->matrix[5] * scale / BATTLE_RENDER_SCALE_ONE;
                transform->matrix[9] =
                    transform->matrix[9] * scale / BATTLE_RENDER_SCALE_ONE;
                transform->matrix[13] =
                    transform->matrix[13] * scale / BATTLE_RENDER_SCALE_ONE;
            }

            if (model->rotation_z != 0) {
                int angle = model->rotation_z >> 4;

                MTX_ApplyRotZ44(FX_SinCosTable_[angle * 2 + 1],
                                FX_SinCosTable_[angle * 2],
                                &transform->matrix[0],
                                &transform->matrix[1]);
            }

            {
                BattlePosition position;

                BattlePosition_StoreViewRelative(
                    &position,
                    object->x,
                    (s16)(object->y - object->z),
                    (s16)(object->effect_anchor_z +
                          16 * (256 - object->y)),
                    object->flags.bits.use_raw_position,
                    object->flags.bits.use_alternate_model);
                transform->x =
                    (transform->x << 8) / BATTLE_RENDER_MATRIX_ONE +
                    ((position.x + model->animation_offset_x +
                      object->property_102) << 8);
                transform->y =
                    (transform->y << 8) / BATTLE_RENDER_MATRIX_ONE +
                    ((position.y + model->animation_offset_y +
                      object->property_103) << 8);
                transform->z =
                    transform->z / BATTLE_RENDER_MATRIX_ONE + position.z;
            }

            if (model->animation_state_bits.state !=
                    BATTLE_RENDER_STATE_NONE &&
                model->animation_state_bits.state <
                    BATTLE_RENDER_STATE_END) {
                (*(u16 *)(gBattleContext + BATTLE_RENDER_FRAME_OFFSET))++;
                if (*(u16 *)(gBattleContext + BATTLE_RENDER_FRAME_OFFSET) >=
                    BATTLE_RENDER_FRAME_LIMIT) {
                    *(u16 *)(gBattleContext + BATTLE_RENDER_FRAME_OFFSET) =
                        BATTLE_RENDER_FRAME_FIRST;
                }
                model->animation_state =
                    (model->animation_state & ~0xFC0) |
                    ((*(u16 *)(gBattleContext + BATTLE_RENDER_FRAME_OFFSET) &
                      0x3F) << 6);
            } else {
                model->animation_state &= ~0xFC0;
            }

            {
                int handled = 0;

                if (object->render_override != 0 &&
                    object->render_override->render != 0) {
                    handled = object->render_override->render(object, 0);
                }
                if (!handled) {
                    switch (object->flags.bits.render_mode) {
                    case 0:
                        if (((*(u32 *)(gBattleContext +
                                       BATTLE_RUNTIME_FLAGS_OFFSET) << 20) >>
                             31) == 0) {
                            model->prepare_render();
                        }
                        model->draw(0, 0, 0);
                        break;
                    case 1:
                        transform->z = 0;
                        func_ov002_020ad880(
                            transform,
                            32 - model->animation_state_bits.state);
                        break;
                    case 2:
                        func_ov002_020bc670(object);
                        break;
                    }
                }
            }

            MTX_Identity44_(model->get_sprite_transform()->matrix);
            model->owner_render_state = object->render_state;
        }
    }

    *(u8 **)(gBattleContext + BATTLE_RENDER_BUFFER_OFFSET) =
        gBattleContext + BATTLE_RENDER_BUFFER_START_OFFSET;
    if (((*(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) << 25) >>
         31) == 0) {
        return func_ov002_0206a02c();
    }
    return (int)gBattleContext;
}
