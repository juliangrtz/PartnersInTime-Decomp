#include "attack_common_internal.h"

extern "C" {
void Overlay10Party_GetSparkPositions(BattlePosition *sprite, BattlePosition *model, BattlePartyActor *actor)
{
    BattleSceneObject *object = actor->actor.scene_object;
    const s8 *offset = data_ov010_020c2a64[actor->formation_index];
    BattlePosition position;
    BattlePosition_StoreViewRelative(
        &position, object->x, (s16)(actor->actor.scene_object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - actor->actor.scene_object->y)),
        actor->actor.scene_object->flags.bits.use_raw_position, object->flags.bits.use_alternate_model);
    sprite->x = position.x + offset[0];
    sprite->y = position.y + offset[1];
    sprite->z = position.z + 24;
    Overlay10Party_GetEffectPosition((s16 *)&position, actor, 1);
    BattlePosition_StoreViewRelative(model, position.x, (s16)(position.y - position.z),
                                     (s16)(object->effect_anchor_z + 16 * (256 - position.y)),
                                     object->flags.bits.use_raw_position,
                                     object->flags.bits.use_alternate_model);
}

int Overlay10Enemy_GetCenter(s16 *center)
{
    int min_x, max_x, min_y, max_y, count;
    min_x = 0x7fffffff;
    max_x = (int)0x80000000;
    count = 0;
    min_y = 0x7fffffff;
    max_y = (int)0x80000000;
    for (int i = 0; i < 6; ++i) {
        BattleActor *actor = BattleActor_GetById((u16)(i + 60));
        if (Overlay10Enemy_IsSelectable((u16)(i + 60))) {
            ++count;
            int x = actor->scene_object->x, y = actor->scene_object->y;
            if (min_x > x)
                min_x = x;
            if (max_x < x)
                max_x = x;
            if (min_y > y)
                min_y = y;
            if (max_y < y)
                max_y = y;
        }
    }
    center[0] = min_x + (max_x - min_x) / 2;
    center[1] = min_y + (max_y - min_y) / 2;
    return count;
}

int Overlay10Attack_CalculateDamage(BattlePartyActor *actor, int item, int argument, int target, int option)
{
    return BattleDamage_CalculateAttack(actor, item, argument, target, 1, option);
}

void Overlay10Party_AverageStats(BattlePartyActor *output, int count)
{
    int max_hp = 0, hp = 0, speed = 0, power = 0, defense = 0, unknown = 0, level = 0;
    for (int i = 0; i < count; ++i) {
        BattleActor *actor = BattleActor_GetById((u16)(i + 56));
        max_hp += actor->max_hp;
        hp += actor->current_hp;
        level += actor->flag_bits.level;
        speed += actor->speed;
        power += actor->power;
        defense += actor->defense;
        unknown += actor->unk_00e;
    }
    output->actor.max_hp = max_hp / count;
    output->actor.current_hp = hp / count;
    output->actor.speed = speed / count;
    output->actor.power = power / count;
    output->actor.defense = defense / count;
    output->actor.unk_00e = unknown / count;
    output->actor.flag_bits.level = level / count;
    output->formation_index = count;
}
}
