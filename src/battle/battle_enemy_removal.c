#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_enemy_defeat.h>
#include <game/battle_scene.h>

enum { BATTLE_EFFECT_SCALE = 256 };

void BattleEnemy_UpdateDefeatRemoval(BattleAITask *base_task)
{
    BattleEnemyDefeatTask *task = (BattleEnemyDefeatTask *)base_task;
    BattleEnemyDefeatPayload *payload = &task->data;
    BattleSceneObject *object = payload->object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |= 1 << 2;
    if (payload->timer > 0) {
        payload->timer--;
    }
    if (payload->timer > 0) {
        return;
    }

    BattleActor_GetById(object->actor_id)->current_hp = 0;
    BattleSceneObject_SetAnimation(object, -1, -1);
    {
        BattleModel *model = BattleSceneObject_GetActiveModel(BattleSceneObject_GetById(object->actor_id));

        if (model != 0) {
            model->flags &= ~BATTLE_MODEL_FLAG_10;
            model->flags &= ~BATTLE_MODEL_FLAG_11;
            model->scale_x = BATTLE_EFFECT_SCALE;
            model->scale_y = BATTLE_EFFECT_SCALE;
            model->rotation_z = 0;
            model->animation_state_bits.state = 31;
        }
    }
    task->callback = 0;
}

void BattleEnemy_UpdateDelayedDefeatRemoval(BattleAITask *base_task)
{
    BattleEnemyDefeatTask *task = (BattleEnemyDefeatTask *)base_task;
    BattleEnemyDefeatPayload *payload = &task->data;
    BattleSceneObject *object = payload->object;

    *(u32 *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET) |= 1 << 2;
    if (payload->timer > 0) {
        payload->timer--;
    }
    if (payload->timer > 0) {
        return;
    }

    BattleActor_GetById(object->actor_id)->current_hp = 0;
    object->primary_model = 0;
    task->callback = 0;
}
