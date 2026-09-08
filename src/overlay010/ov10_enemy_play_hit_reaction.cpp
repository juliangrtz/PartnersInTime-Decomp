#include "attack_common_internal.h"

extern "C" {
void Overlay10Enemy_PlayHitReaction(int actor_id, int effect_id)
{
    int busy = func_ov002_02071674(actor_id);
    BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
    BattleEnemyActor *actor = (BattleEnemyActor *)BattleActor_GetEnemySlot(actor_id);
    if (!busy) {
        int resource = object->resource->resource_id;
        s16 previous = object->previous_base_animation_id;
        if (resource != (int)0xc1000082 && resource != (int)0xc10000a4) {
            actor->reaction_state.impact_offset_x += object->property_102;
            actor->reaction_state.impact_offset_y += object->property_103;
            object->property_102 = 0;
            object->property_103 = 0;
        }
        BattleSceneObject_SetAnimation(object, actor->defeat_animation_id, -1);
        if (BattleActor_GetById(object->actor_id)->flag_bits.hit_locked)
            object->previous_base_animation_id = previous;
    }
    BattleDamage_StartEnemyReaction(object, effect_id);
}
}
