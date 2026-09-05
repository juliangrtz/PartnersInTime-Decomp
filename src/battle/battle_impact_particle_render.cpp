#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_impact_effect.h>

enum BattleImpactParticleRenderOffset {
    BATTLE_IMPACT_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_IMPACT_VIEW_Y_OFFSET = 0xCB9E,
    BATTLE_IMPACT_ANIMATION_FRAME_OFFSET = 0x68D6
};

enum BattleImpactParticleRenderConstant {
    BATTLE_IMPACT_SCALE_ONE = 256,
    BATTLE_IMPACT_MATRIX_ONE = 4096,
    BATTLE_IMPACT_STATIC_FRAME = 31,
    BATTLE_IMPACT_FRAME_FIRST = 18,
    BATTLE_IMPACT_FRAME_LIMIT = 53
};

extern "C" {
extern const s16 FX_SinCosTable_[];
extern BattleSpriteTransform data_ov002_020c0cd8;

void MTX_ApplyRotZ44(int cosine, int sine,
                     s32 *x_column, s32 *y_column);
void func_0202cc58(const void *source, void *destination, u32 size);
void func_0202cd2c(const void *source, void *destination, u32 size);
}

extern "C" void BattleImpactParticle_Render(
    BattleImpactParticlePayload *particle, BattleModel *model,
    s16 x, s16 y, s16 z) {
    BattleSpriteTransform *transform;
    BattleSpriteTransform *saved_transform = &data_ov002_020c0cd8;
    s32 *matrix;
    int scale_x;
    int scale_y;
    int rotation;
    u8 saved_animation_state;
    u8 saved_animation_id;
    u8 saved_property_056;

    if (model == 0 || particle->render_bits.size < 1) {
        return;
    }

    transform = model->get_sprite_transform();
    if (transform < saved_transform) {
        func_0202cd2c(transform, saved_transform, sizeof(*transform));
    } else {
        func_0202cc58(transform, saved_transform, sizeof(*transform));
    }
    matrix = transform->matrix;

    rotation = particle->rotation_z_high << 8;
    scale_x = particle->scale_x;
    scale_y = particle->scale_y;

    if (particle->render_bits.flip_x) {
        matrix[0] = -matrix[0];
        matrix[4] = -matrix[4];
        matrix[8] = -matrix[8];
        matrix[12] = -matrix[12];
    }
    if (particle->render_bits.flip_y) {
        matrix[1] = -matrix[1];
        matrix[5] = -matrix[5];
        matrix[9] = -matrix[9];
        matrix[13] = -matrix[13];
    }

    if (scale_x != BATTLE_IMPACT_SCALE_ONE) {
        matrix[0] = matrix[0] * scale_x / BATTLE_IMPACT_SCALE_ONE;
        matrix[4] = matrix[4] * scale_x / BATTLE_IMPACT_SCALE_ONE;
        matrix[8] = matrix[8] * scale_x / BATTLE_IMPACT_SCALE_ONE;
        matrix[12] = matrix[12] * scale_x / BATTLE_IMPACT_SCALE_ONE;
    }
    if (scale_y != BATTLE_IMPACT_SCALE_ONE) {
        matrix[1] = matrix[1] * scale_y / BATTLE_IMPACT_SCALE_ONE;
        matrix[5] = matrix[5] * scale_y / BATTLE_IMPACT_SCALE_ONE;
        matrix[9] = matrix[9] * scale_y / BATTLE_IMPACT_SCALE_ONE;
        matrix[13] = matrix[13] * scale_y / BATTLE_IMPACT_SCALE_ONE;
    }

    if (rotation != 0) {
        int angle = rotation >> 4;

        MTX_ApplyRotZ44(FX_SinCosTable_[angle * 2 + 1],
                        FX_SinCosTable_[angle * 2],
                        &matrix[0], &matrix[1]);
    }

    x -= *(s16 *)(gBattleContext + BATTLE_IMPACT_VIEW_X_OFFSET);
    y -= *(s16 *)(gBattleContext + BATTLE_IMPACT_VIEW_Y_OFFSET);
    transform->x =
        (transform->x << 8) / BATTLE_IMPACT_MATRIX_ONE + (x << 8);
    transform->y =
        (transform->y << 8) / BATTLE_IMPACT_MATRIX_ONE + (y << 8);
    if (z < 0) {
        z = 0;
    }
    transform->z = transform->z / BATTLE_IMPACT_MATRIX_ONE + z;

    saved_animation_state = model->animation_state_bits.state;
    saved_animation_id = model->animation_id;
    saved_property_056 = model->property_056;
    if (particle->render_bits.size == BATTLE_IMPACT_STATIC_FRAME) {
        model->animation_state &= 0xF03F;
        model->animation_state_bits.state = BATTLE_IMPACT_STATIC_FRAME;
    } else {
        u16 *frame = (u16 *)(gBattleContext +
                             BATTLE_IMPACT_ANIMATION_FRAME_OFFSET);

        (*frame)++;
        if (*frame >= BATTLE_IMPACT_FRAME_LIMIT) {
            *frame = BATTLE_IMPACT_FRAME_FIRST;
        }
        model->animation_state =
            (model->animation_state & 0xF03F) | ((*frame & 0x3F) << 6);
        model->animation_state_bits.state = particle->render_bits.size;
    }
    model->animation_id = particle->animation_id;
    model->property_056 = particle->model_property_056;
    model->draw(0, 0, 0);

    if (saved_transform < transform) {
        func_0202cd2c(saved_transform, transform, sizeof(*transform));
    } else {
        func_0202cc58(saved_transform, transform, sizeof(*transform));
    }
    model->animation_state =
        (model->animation_state & ~0x1F) | (saved_animation_state & 0x1F);
    model->animation_id = saved_animation_id;
    model->property_056 = saved_property_056;
}
