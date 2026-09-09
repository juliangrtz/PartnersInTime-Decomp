extern "C" {
#include <game/battle_feedback.h>
#include <game/battle_context.h>
#include <game/battle_damage.h>
#include <game/battle_damage_number.h>
int _s32_div_f(int, int);
void func_0200940c(BattleModel *, int);
}

void BattleParty_PlayFormationSound(BattlePartyActor *actor, int adult_sound, int baby_sound)
{
    const u16 first_baby_bias = (u16)-2;
    if ((u16)(actor->formation_index + first_baby_bias) <= 1)
        BattleSound_Play(baby_sound, 0, 0, 0);
    else
        BattleSound_Play(adult_sound, 0, 0, 0);
}

int BattleActor_HasTrait1(int actor_id)
{
    return ((BattleEnemyActor *)BattleActor_GetById(actor_id))->state_flag_bits.traits == 1;
}

void BattleActor_SetStateFlag03(int actor_id)
{
    ((BattleEnemyActor *)BattleActor_GetById(actor_id))->state_flag_bits.flag_03 = 1;
}

static inline u8 AnimationActive(BattleModel *model)
{
    return model->flag_bits.animation_active;
}

int BattleParty_RestoreIdleAnimation(BattlePartyActor *actor, BattleSceneObject *object)
{
    if (!object)
        object = actor->actor.scene_object;
    int animation = 0;
    int resource = actor->formation_index & 1;
    if (BattleActor_IsHpAtMostQuarter(&actor->actor))
        animation = 22;
    if (object->actor_id >= 58)
        return animation;
    if (object->resource->object_data_id != resource ||
        animation != BattleSceneObject_GetActiveModel(object)->get_animation_id() ||
        !AnimationActive(BattleSceneObject_GetActiveModel(object))) {
        BattleEntity_BindResource(object->actor_id, (u16)resource);
        BattleSceneObject_SetAnimation(object, animation, -1);
    }
    BattleSceneObject_SetAnimationMode01(object, 1);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    model->flag_bits.facing_left = 0;
    model->flag_bits.flip_y = 0;
    func_0200940c(model, 256);
    return animation;
}

void BattleSceneObject_SetAnimationMode01(BattleSceneObject *object, int mode_zero)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    if (model) {
        if (mode_zero)
            model->flag_bits.animation_mode = 0;
        else
            model->flag_bits.animation_mode = 1;
        model->flag_bits.unknown_09 = 0;
    }
}

int BattleAnimation_GetDurationThroughFrame(const GameGraphicsResource *resource, int animation, int frame,
                                            int speed)
{
    const BattleModelFrameEntry *frames = (const BattleModelFrameEntry *)GameGraphics_GetSection(resource, 2);
    const BattleModelAnimationData *animations =
        (const BattleModelAnimationData *)GameGraphics_GetSection(resource, 1);
    int end = animations[animation].end_frame, total = 0;
    if (frame != -1)
        end = animations[animation].start_frame + frame;
    else
        end -= 1;
    for (int i = animations[animation].start_frame; i <= end; ++i)
        total += frames[i].value.bits.offset;
    return _s32_div_f(total << 8, speed);
}

int BattleActor_IsDamageImmune(u32 actor_id)
{
    /* The original check includes the endpoint immediately after the eight enemy slots. */
    return actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
           actor_id <= BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT &&
           ((BattleEnemyActor *)BattleActor_GetEnemySlot(actor_id))->state_flag_bits.damage_immune;
}

void BattleParty_ApplyNonfatalDamageFeedback(BattlePartyActor *actor, int unused, BattleSceneObject *object)
{
    if (!object)
        object = actor->actor.scene_object;
    BattleEffect_SetVariant(-1);
    BattleFeedback_SpawnVariant(object, 0, 0, 1);
    if (actor->actor.current_hp > 1)
        BattleActor_ApplyDamage(actor->actor.scene_object, 1);
    BattlePosition position;
    BattlePosition_StoreViewRelative(&position, object->x, (s16)(object->y - object->z),
                                     (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
                                     object->flags.bits.use_raw_position,
                                     object->flags.bits.use_alternate_model);
    BattleDamageNumberEffect *number =
        (BattleDamageNumberEffect *)BattleDamage_SpawnNumber(1, position.x, position.y, 5, 0);
    if ((actor->formation_index & 1) == 0)
        number->party_style = 12;
    if ((actor->formation_index & 1) == 1)
        number->party_style = 13;
    BattleScreenEffect_StartPrimary(1, 60, 0);
}
