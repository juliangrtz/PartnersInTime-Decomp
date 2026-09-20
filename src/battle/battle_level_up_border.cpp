/* Level-up border fade and drawing, overlay 2, 0x02095E00-0x02095F6C. */
#include <game/battle_ai.h>
#include <game/battle_effect.h>

extern "C" void BattleLevelUp_DrawBorder(BattleAITask *task) {
    /* This callback uses the task payload slot for its scene object. */
    BattleModel *model = ((BattleSceneObject *)task->state)->primary_model;
    if (!model) {
        task->callback = 0;
        return;
    }
    int alpha = model->animation_state_bits.state;
    if (model->flag_bits.animation_active)
        alpha += 2;
    else
        alpha -= 2;
    if (alpha <= 0) {
        task->callback = 0;
        return;
    }
    if (alpha > 31)
        alpha = 31;
    model->animation_state_bits.state = (u16)alpha;
    model->owner_render_state = 32767;
    BattleSpriteTransform *transform = model->get_sprite_transform();
    /* Reset the 3x3 basis; keep the other homogeneous matrix fields. */
    transform->matrix[0] = 4096;
    transform->matrix[1] = 0;
    transform->matrix[2] = 0;
    transform->matrix[4] = 0;
    transform->matrix[5] = 4096;
    transform->matrix[6] = 0;
    transform->matrix[8] = 0;
    transform->matrix[9] = 0;
    transform->matrix[10] = 4096;
    transform->z = 32;
    model->prepare_render();
    /* Sixteen 16-pixel tiles across the top and bottom of the screen. */
    int i = 0;
    do {
        transform->x = (8 + 16 * i) << 8;
        transform->y = 2048;
        model->draw(0, 0, 0);
        transform->y = 47104;
        model->draw(0, 0, 0);
        ++i;
    } while (i < 16);
}
