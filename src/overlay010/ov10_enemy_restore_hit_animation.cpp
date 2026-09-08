#include "attack_common_internal.h"

extern "C" {
void Overlay10Enemy_RestoreHitAnimation(int actor_id)
{
    BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
    BattleEnemyActor *actor = (BattleEnemyActor *)BattleActor_GetEnemySlot(actor_id);
    int hit_animation = actor->hit_animation_id + object->animation_variant_offset;
    if (!actor->state_flag_bits.damage_immune) {
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        if (hit_animation == model->get_animation_id()) {
            int animation = object->previous_base_animation_id;
            if (animation < 0) {
                object->animation_variant_offset += animation;
                BattleSceneObject_SetAnimation(object, 0, -1);
                object->animation_variant_offset -= animation;
            } else
                BattleSceneObject_SetAnimation(object, animation, -1);
            u32 id = object->actor_id;
            if (id >= 60 && id < 68) {
                int resource = object->resource->resource_id;
                if (resource != (int)0xc1000082 && resource != (int)0xc10000a4) {
                    BattleEnemyActor *enemy = (BattleEnemyActor *)BattleActor_GetEnemySlot(id);
                    object->property_102 += enemy->reaction_state.impact_offset_x;
                    object->property_103 += enemy->reaction_state.impact_offset_y;
                    enemy->reaction_state.impact_offset_x = 0;
                    enemy->reaction_state.impact_offset_y = 0;
                }
            }
        }
    }
    if (!(*(u32 *)(gBattleContext + 54176) << 13 >> 31)) {
        if (!*((s8 *)actor + 56))
            BattleDamage_StartEffectReaction(object, 83);
    }
}
}
