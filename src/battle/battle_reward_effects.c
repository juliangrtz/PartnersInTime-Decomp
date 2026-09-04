#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_effect.h>

extern u8 *gSaveData;
extern volatile const s8 gBattlePartyHealingEffectOffsets[];
extern volatile const s8 gBattlePartyHealingNumberOffsets[];

enum BattleRewardContextOffset {
    BATTLE_REWARD_SCENE_OBJECT_OFFSET = 0x675C,
    BATTLE_REWARD_MARIO_EFFECT_OWNER_OFFSET = 0x6578,
    BATTLE_REWARD_LUIGI_EFFECT_OWNER_OFFSET = 0x6594,
    BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET = 0xD398,
    BATTLE_REWARD_COIN_EFFECT_OFFSET = 0xD39C
};

enum BattleRewardType {
    BATTLE_REWARD_COINS = 0,
    BATTLE_REWARD_EXPERIENCE = 1
};

enum BattleRewardSaveOffset {
    BATTLE_REWARD_PARTY_FORM_OFFSET = 0x558
};

int BattleParty_ShowHealingEffect(BattleActor *base_actor, int amount) {
    BattlePartyActor *actor = (BattlePartyActor *)base_actor;
    BattlePosition position;
    int position_index;
    int table_index;
    int next_table_index;

    switch (actor->formation_index) {
    case 0:
        position_index = 0;
        break;
    case 1:
        position_index = 1;
        break;
    case 2:
        position_index = 2;
        break;
    case 3:
        position_index = 3;
        break;
    case 4:
        position_index = 4;
        break;
    case 5:
        position_index = 5;
        break;
    }

    {
        BattleSceneObject *object = actor->actor.scene_object;

        BattlePosition_StoreViewRelative(
            &position,
            object->x,
            (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model
        );
    }

    table_index = position_index * 2;
    next_table_index = table_index + 1;
    BattleSpriteEffect_Spawn(
        73,
        (s16)(position.x +
              gBattlePartyHealingEffectOffsets[table_index]),
        (s16)(position.y +
              gBattlePartyHealingEffectOffsets[next_table_index]),
        (s16)(position.z - 16),
        256
    );
    BattleModelEffect_Spawn(
        134,
        actor->actor.scene_object,
        gBattlePartyHealingEffectOffsets[table_index],
        gBattlePartyHealingEffectOffsets[next_table_index],
        -16,
        256
    );
    BattleDamage_SpawnNumber(
        amount,
        position.x + gBattlePartyHealingNumberOffsets[table_index],
        position.y + gBattlePartyHealingNumberOffsets[next_table_index],
        6,
        0
    );

    if (amount > 0) {
        u8 *effect_record;

        if (actor->actor.scene_object->actor_id == BATTLE_ACTOR_PARTY_FIRST) {
            effect_record =
                gBattleContext + BATTLE_REWARD_MARIO_EFFECT_OWNER_OFFSET;
        } else {
            effect_record =
                gBattleContext + BATTLE_REWARD_LUIGI_EFFECT_OWNER_OFFSET;
        }
        BattleModelEffect_SpawnAttached(
            (BattleEffect **)(effect_record + 0x14),
            866,
            *(BattleSceneObject **)effect_record,
            12,
            -8,
            -2,
            384
        );
    }

    return BattleSound_Play(333, 0, 0, 0);
}

BattleEffect *BattleReward_EnsureCounterEffect(int reward_type) {
    if (reward_type == BATTLE_REWARD_EXPERIENCE) {
        BattlePosition position;
        BattleSceneObject *reward_object;
        BattleEffect *effect;

        effect = *(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET);
        if (effect != 0) {
            return effect;
        }

        reward_object = *(BattleSceneObject **)(
            gBattleContext + BATTLE_REWARD_SCENE_OBJECT_OFFSET
        );
        BattlePosition_StoreViewRelative(
            &position,
            reward_object->x,
            (s16)(reward_object->y - reward_object->z),
            (s16)(reward_object->effect_anchor_z +
                  16 * (256 - reward_object->y)),
            reward_object->flags.bits.use_raw_position,
            reward_object->flags.bits.use_alternate_model
        );
        if (*(s16 *)(gSaveData + BATTLE_REWARD_PARTY_FORM_OFFSET) == 2) {
            position.x += 100;
            position.y += 44;
        } else {
            position.x += 100;
            position.y += 60;
        }

        effect = BattleSpriteEffect_Spawn(
            475, position.x, position.y, 16, 256
        );
        effect->completion_callback = BattleReward_AdvanceCounterEffect;
        effect->owner_slot = (BattleEffect **)(
            gBattleContext + BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET
        );
        *(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET
        ) = effect;
        return effect;
    } else {
        BattlePosition position;
        BattleSceneObject *reward_object;
        BattleEffect *effect;

        effect = *(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_COIN_EFFECT_OFFSET);
        if (effect != 0) {
            return effect;
        }

        reward_object = *(BattleSceneObject **)(
            gBattleContext + BATTLE_REWARD_SCENE_OBJECT_OFFSET
        );
        BattlePosition_StoreViewRelative(
            &position,
            reward_object->x,
            (s16)(reward_object->y - reward_object->z),
            (s16)(reward_object->effect_anchor_z +
                  16 * (256 - reward_object->y)),
            reward_object->flags.bits.use_raw_position,
            reward_object->flags.bits.use_alternate_model
        );
        position.x += 196;
        position.y += 92;

        effect = BattleSpriteEffect_Spawn(
            475, position.x, position.y, 16, 256
        );
        effect->completion_callback = BattleReward_AdvanceCounterEffect;
        effect->owner_slot = (BattleEffect **)(
            gBattleContext + BATTLE_REWARD_COIN_EFFECT_OFFSET
        );
        *(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_COIN_EFFECT_OFFSET
        ) = effect;
        return effect;
    }
}

BattleEffect *BattleReward_AdvanceCounterEffect(BattleEffect *effect) {
    BattleEffect *next = BattleSpriteEffect_Spawn(
        476,
        (s16)(effect->x - *(s16 *)(gBattleContext + 0xCB9C)),
        (s16)(effect->y - *(s16 *)(gBattleContext + 0xCB9E)),
        effect->z,
        256
    );

    next->completion_callback = effect->completion_callback;
    next->owner_slot = effect->owner_slot;
    *next->owner_slot = next;
    return next;
}

void BattleReward_ClearCounterEffects(void) {
    BattleEffect *effect = *(BattleEffect **)(
        gBattleContext + BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET
    );

    if (effect != 0) {
        effect->update_callback = 0;
        (*(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET
        ))->completion_callback = 0;
        *(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_EXPERIENCE_EFFECT_OFFSET
        ) = 0;
    }

    effect = *(BattleEffect **)(
        gBattleContext + BATTLE_REWARD_COIN_EFFECT_OFFSET
    );
    if (effect != 0) {
        effect->update_callback = 0;
        (*(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_COIN_EFFECT_OFFSET
        ))->completion_callback = 0;
        *(BattleEffect **)(
            gBattleContext + BATTLE_REWARD_COIN_EFFECT_OFFSET
        ) = 0;
    }
}
