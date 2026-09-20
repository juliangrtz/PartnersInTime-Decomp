/* Smash Eggs pair-hit resolution, overlay 15, 0x020C4728-0x020C4A68. */
#include "actor_internal.h"
#include <game/battle_effect.h>
#include <game/overlay010_attack.h>
extern "C" {
#include <game/battle_damage.h>


extern u16 data_ov015_020c5f64[];
extern s16 data_ov015_020c5e64[];
extern int data_ov015_020c5e78[];
extern s8 data_ov015_020c5e9c[][2];
extern const int *data_ov015_020c5e6c;
void func_ov015_020c337c(Overlay15AttackObjectPairState *pair,
    Overlay15AttackModelController *other, int index_offset);

void Overlay15Attack_ResolvePairHit(Overlay15AttackModelController *state,
    Overlay15AttackModelController *other, Overlay15AttackObjectPairState *pair)
{
    Overlay15AttackContext *context;
    Overlay15AttackModelOwner *owner = state->primary;
    context = data_ov002_020c0710;
    BattleSceneObject *object = owner->object;
    u16 hit_actor = (owner->formation_index & 1) ? 9 : 8;
    u32 phase = state->phase_flags.bits.phase;
    if (phase >= 8 && phase <= 10) {
        BattleHitDescriptor_DisableByActor(hit_actor);
        BattleSpriteEffect_SpawnRelative(14, pair->primary, 0, 0, 0, 256);
        int index = Overlay15Attack_ClampContextIndex(0);
        if (78 * pair->hit_count + data_ov015_020c5fa0[index] <= 380) {
            ++pair->hit_count;
        }
        func_ov015_020c337c(pair, other, 0);
        BattleSound_Play(284, 0, 0, 0);
        BattleScreenEffect_StartSecondary(6, 1, 0);
        pair->flags.bits.alternate_actor =
            (u8)(pair->flags.bits.alternate_actor ^ 1);
    } else {
        BattleSound_Play(63, 0, 0, 0);
        BattleScreenEffect_StartSecondary(5, 1, 0);
        state->phase_flags.raw |= 128;
        BattleHitDescriptor_DisableByActor(40);
        int target_id = Overlay15Attack_SelectRandomEnemy();
        BattleActor *target = BattleActor_GetById(target_id);
        /* The original uses double arithmetic before truncating to integer. */
        int power = (int)((double)context->power +
            64.0 * (double)pair->hit_count);
        u8 hit_bonus;
        int damage;
        if (BattleParty_RollHitBonus((BattlePartyActor *)owner, target)) {
            hit_bonus = 1;
            damage = Overlay10Attack_CalculateDamage(
                (BattlePartyActor *)owner, power, 6, target, 0);
        } else {
            hit_bonus = 0;
            damage = Overlay10Attack_CalculateDamage(
                (BattlePartyActor *)owner, power, 1, target, 0);
        }
        int index = Overlay15Attack_ClampContextIndex(0);
        u32 roll = Random_NextModulo(100);
        if (data_ov015_020c5f64[index] > roll) {
            pair->flags.bits.alternate_actor = (u8)Random_NextModulo(2);
        } else {
            pair->flags.bits.alternate_actor =
                (u8)(pair->flags.bits.alternate_actor ^ 1);
        }
        BattleSpriteEffect_SpawnRelative(14, pair->primary, 0, 0, 0, 256);
        /* Projected damage is signed; the launch receives its unsigned low half. */
        Overlay10Enemy_ApplyProjectedDamage(
            (Overlay10EnemyState *)context, target_id, (s16)damage);
        Overlay10Attack_ShowRating(data_ov015_020c5e64, data_ov015_020c5e9c,
            owner->formation_index, object, 0, data_ov015_020c5e6c);
        Overlay15Attack_BeginPairLaunch(pair, target_id, (u16)damage, hit_bonus, 0);
        context->power = Overlay10_SelectThresholdValue(data_ov015_020c5e78,
            Overlay15Attack_ClampContextIndex(0), data_ov015_020c5e64);
        ++context->index;
        pair->hit_count = 0;
    }
}
}
