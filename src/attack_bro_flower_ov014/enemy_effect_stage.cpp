/* Bro Flower enemy effect stages, overlay 14, 0x020C30A0-0x020C3388. */
#include "flower_internal.h"
extern "C" void Overlay14Attack_UpdateEnemyEffectStage(u16 id)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    BattleSceneObject *enemy = BattleSceneObject_GetById(id);
    int index = id - 60;
    int level = work->enemy_effect_stage[index];
    int changed;
    /* This shared counter holds successful hits during the Flower attack. */
    int hits = work->common.scale_steps[index];
    u8 *stage = &work->enemy_effect_stage[index];
    changed = 0;
    /* Only raise the stage; one hit can cross more than one threshold. */
    if (level < 3 && hits >= 12) {
        level = 3;
        changed = 1;
    } else if (level < 2 && hits >= 6) {
        level = 2;
        changed = 1;
    } else if (level < 1 && hits >= 1) {
        level = 1;
        changed = 1;
    }
    if (!changed)
        return;
    *stage = level;
    s16 scale_x, scale_y;
    if (work->variant) {
        scale_x = 256;
        scale_y = 256;
    } else {
        scale_x = 128;
        scale_y = 153;
    }
    s16 position[2];
    switch (level) {
    case 3: {
        func_ov014_020c348c(position, enemy, 3);
        BattleSceneObject *effect = &work->effects[index][2];
        BattleSceneObject_AdjustPosition(effect,
            enemy->x + position[0] - effect->x,
            enemy->y - effect->y,
            enemy->z - position[1] - effect->z);
        work->effects[index][2].effect_anchor_z = enemy->effect_anchor_z - 24;
        BattleSceneObject_SetAnimation(effect, 0, -1);
        work->effects[index][2].primary_model->scale_x = scale_x;
        work->effects[index][2].primary_model->scale_y = scale_y;
        break;
    }
    case 2: {
        func_ov014_020c348c(position, enemy, 2);
        BattleSceneObject *effect = &work->effects[index][1];
        BattleSceneObject_AdjustPosition(effect,
            enemy->x + position[0] - effect->x,
            enemy->y - effect->y,
            enemy->z - position[1] - effect->z);
        work->effects[index][1].effect_anchor_z = enemy->effect_anchor_z - 16;
        BattleSceneObject_SetAnimation(effect, 0, -1);
        work->effects[index][1].primary_model->scale_x = scale_x;
        work->effects[index][1].primary_model->scale_y = scale_y;
        break;
    }
    case 1: {
        func_ov014_020c348c(position, enemy, 1);
        BattleSceneObject *effect = &work->effects[index][0];
        BattleSceneObject_AdjustPosition(effect,
            enemy->x + position[0] - effect->x,
            enemy->y - effect->y,
            enemy->z - position[1] - effect->z);
        work->effects[index][0].effect_anchor_z = enemy->effect_anchor_z - 8;
        BattleSceneObject_SetAnimation(effect, 0, -1);
        work->effects[index][0].primary_model->scale_x = scale_x;
        work->effects[index][0].primary_model->scale_y = scale_y;
        break;
    }
    }
}
