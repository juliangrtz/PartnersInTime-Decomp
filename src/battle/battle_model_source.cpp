#include <game/battle_model_animation.h>

/* Positive IDs select the alternate model; zero and negative sentinels pass through. */
BattleModel *BattleModelAnimation_ResolveSource(int source, BattleModel **resolved_source)
{
    if (source <= 0) {
        *resolved_source = (BattleModel *)source;
        return (BattleModel *)source;
    }
    BattleModel *model = BattleSceneObject_GetById((u16)source)->alternate_model;
    *resolved_source = model;
    return model;
}
