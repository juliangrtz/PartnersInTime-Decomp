/* Bro/Ice Flower impact feedback and status chances, 0x020C3AA4-0x020C3CE4. */
#include "flower_internal.h"
#include <game/battle_context.h>
#include <game/battle_feedback.h>
#include <game/battle_status.h>

extern "C" {
void Overlay10Enemy_PlayHitReaction(int actor_id, int effect_id);
int Overlay10Enemy_CalculateScaledValue(Overlay10EnemyState *state, int actor_id,
                                       int scale, int maximum);

void Overlay14Projectile_ApplyImpact(Overlay14Projectile *projectile)
{
    Overlay14Work *work;
    u16 id = projectile->target_id;
    work = (Overlay14Work *)data_ov002_020c0710;
    BattleSceneObject *enemy = BattleSceneObject_GetById(id);
    BattleActor_GetById(id);
    int x = projectile->object.x - enemy->x;
    int y = enemy->z - projectile->object.z;
    if (projectile->flags.level < 2)
        return;

    BattleEffect_SetVariant(1);
    if (work->variant) {
        BattleSpriteEffect_SpawnRelative(536, &projectile->object, 0, 0, 0, 256);
        BattleModelEffect_SpawnRelative(411, &projectile->object, 0, 0, 0, 0, 256);
    } else {
        BattleSpriteEffect_SpawnRelative(288, &projectile->object, 0, 0, 0, 256);
        BattleModelEffect_SpawnRelative(411, &projectile->object, 0, 0, 0, 0, 256);
    }
    Overlay10Enemy_PlayHitReaction(id, 12);
    int index = id - 60;
    work->enemy_hit[index] = 1;
    BattleSound_Play(104, 0, 0, 0);
    BattleFeedback_SpawnVariant(enemy, x, y, 1);

    /* The attack initializer supplies only 0 (Fire) or 1 (Ice). */
    int status, magnitude, chance;
    switch (work->variant) {
    case 0:
        status = BATTLE_STATUS_AILMENT_3;
        chance = Overlay10Enemy_CalculateScaledValue((Overlay10EnemyState *)work,
                                                      id, 128, 10);
        magnitude = 0;
        break;
    case 1: {
        int roll = Random_NextModulo(60);
        if (roll < 30)
            status = BATTLE_STATUS_POWER_CHANGE;
        else if (roll < 50)
            status = BATTLE_STATUS_DEFENSE_CHANGE;
        else if (roll < 60)
            status = BATTLE_STATUS_SPEED_CHANGE;
        chance = Overlay10Enemy_CalculateScaledValue((Overlay10EnemyState *)work,
                                                      id, 256, 20);
        magnitude = -30;
        break;
    }
    }
    /* Use projected HP and the pre-increment hit count for the status chance. */
    if (work->common.current_hp[index] > 0)
        BattleStatus_TryApply(BattleActor_GetById(id), status,
                             ((BattleContext *)gBattleContext)->status_duration,
                             magnitude, chance);
    Overlay10Enemy_AddScaleSteps((Overlay10EnemyState *)work, id, 1);
    Overlay14Attack_UpdateEnemyEffectStage(id);
}
}
