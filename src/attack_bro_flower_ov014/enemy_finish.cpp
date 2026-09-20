/* Bro/Ice Flower enemy finish, overlay 14, 0x020C2D24-0x020C30A0. */
#include "flower_internal.h"
#include <game/battle_feedback.h>
extern "C" {
#include <game/battle_damage.h>
/* The caller supplies a second argument that this routine does not read. */
void Overlay14Attack_FinishEnemy(u16 id, int)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    int index = id - 60;
    BattleSceneObject *enemy = BattleSceneObject_GetById(id);
    BattleActor *actor = BattleActor_GetById(id);
    BattleEnemyStatRecord *stats = BattleEnemy_GetStats(id);
    int popup = 7;
    int feedback = 1;
    switch (work->variant) {
    case 0:
        if (stats->trait_bits.unknown_trait_03_04 == 1) {
            popup = 656;
            feedback = 2;
        }
        break;
    case 1:
        if (stats->trait_bits.unknown_trait_03_04 == 2) {
            popup = 656;
            feedback = 2;
        }
        break;
    }
    if (work->enemy_trait[index])
        feedback = 6;
    int i;
    BattleSceneObject *effect = work->effects[index];
    for (i = 0; i < 3; ++i, ++effect)
        BattleSceneObject_SetAnimation(effect, -1, -1);
    if (!work->enemy_hit[index])
        return;
    if (BattleActor_IsDamageImmune(id))
        return;
    if (!Overlay10Enemy_IsSelectable(id))
        return;

    /* Commit the projected damage once, after all projectile hits. */
    int damage = actor->current_hp - work->common.current_hp[index];
    if (actor->damage_flag_bits.force_one_damage)
        damage = 1;
    int sprite, model;
    if (work->variant) {
        sprite = 17;
        model = 36;
    } else {
        sprite = 16;
        model = 36;
    }
    u8 *stage = &work->enemy_effect_stage[index];
    /* Stage is maintained in 0..3; higher stages include the earlier effects. */
    s16 position[2];
    switch (*stage) {
    case 3:
        func_ov014_020c348c(position, enemy, 3);
        BattleSpriteEffect_SpawnRelative(sprite, enemy, position[0], position[1], -16, 256);
        BattleModelEffect_SpawnRelative(model, enemy, 0, position[0], position[1], -16, 256);
        /* fall through */
    case 2:
        func_ov014_020c348c(position, enemy, 2);
        BattleSpriteEffect_SpawnRelative(sprite, enemy, position[0], position[1], -8, 256);
        BattleModelEffect_SpawnRelative(model, enemy, 0, position[0], position[1], -8, 256);
        /* fall through */
    case 0:
    case 1:
        func_ov014_020c348c(position, enemy, 1);
        BattleSpriteEffect_SpawnRelative(sprite, enemy, position[0], position[1], 0, 256);
        BattleModelEffect_SpawnRelative(model, enemy, 0, position[0], position[1], 0, 256);
        func_ov014_020c348c(position, enemy, 2);
        BattleDamage_ApplyToEnemy(enemy, position[0], position[1], damage, 14, popup, 0);
        break;
    }
    BattleEffect_SetVariant((s16)feedback);
    BattleFeedback_SpawnVariant(enemy, position[0], position[1], 0);
    /* Preserve an existing bonus rating over elemental feedback. */
    if (work->feedback != 1) {
        switch (feedback) {
        case 2:
            work->feedback = 2;
            break;
        case 6:
            work->feedback = 1;
            break;
        }
    }
    if (work->rating < *stage)
        work->rating = *stage;
    if (!work->rating)
        work->rating = 1;
}
}
