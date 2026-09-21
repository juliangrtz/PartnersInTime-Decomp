/*
 * Hammer input, impact and return (overlay 21, 0x020C2BCC-0x020C3380).
 *
 * Tracks the two input windows, resolves a swing's damage and visual effects,
 * then moves the actor back using the duration for its recorded result.
 */
extern "C" {
#include <game/battle_damage.h>
#include <game/battle_effect.h>
#include <game/battle_enemy_data.h>
#include <game/battle_feedback.h>
#include <game/battle_status.h>
#include <nitro/fx.h>
}
#include <game/overlay021_attack_state.h>

struct HammerHitEffects {
  int sprite, model, reaction, popup, sound;
  int screen_kind, unknown_18, screen_duration, screen_delay;
};
extern "C" {
extern HammerHitEffects data_ov021_020c3fa0[];
extern u8 *gBattleContext;
extern u16 data_ov002_020be704[];
int BattleDamage_CalculateAttack(BattlePartyActor *, int, int, BattleActor *,
                                 int, int);
void Overlay10Enemy_RestoreHitAnimation(int actor_id);

static inline void ProjectImpact(BattlePosition *position,
                                 BattleSceneObject *object, s16 x, s16 y,
                                 s16 z) {
  x += object->x;
  y += object->y;
  z += object->z;
  BattlePosition_StoreViewRelative(
      position, x, (s16)(y - z),
      (s16)(object->effect_anchor_z + 16 * (256 - y)),
      object->flags.bits.use_raw_position,
      object->flags.bits.use_alternate_model);
}

u32 Overlay21Attack_BeginReturn(Overlay21AttackState *state) {
  BattlePartyActor *actor = state->actor;
  BattleSceneObject *object = state->object;
  BattleEntity_BindResource(object->actor_id,
                            (u16)(actor->formation_index & 1));
  Overlay21Attack_ConfigureAnimation(state, 4, 1);
  BattleModel *model = BattleSceneObject_GetActiveModel(object);
  model->flag_bits.facing_left = 1;
  model->flag_bits.flip_y = 0;
  int dx = actor->actor.unk_018 - object->x;
  int dy = actor->actor.unk_01a - object->y;
  int distance = FX_Sqrt((dx * dx + dy * dy) << 12);
  Overlay21AttackConfig *config = state->config;
  int duration;
  /* A return starts only after one of the three result transitions. */
  switch (state->bits.result) {
  case 1:
    duration = config->success_return_duration;
    break;
  case 2:
    duration = config->miss_return_duration;
    break;
  case 3:
    duration = config->early_return_duration;
    break;
  }
  BattleSceneObject_MoveBy(object, 2, dx, dy, 0, distance * duration / 0x80000);
  BattleSound_Play(61, 0, 0, 0);
  u32 result = (state->flags & ~31) | 12;
  state->flags = result;
  return result;
}

void Overlay21Attack_ApplyHit(Overlay21AttackState *state, int success,
                              int advanced) {
  BattlePartyActor *actor = state->actor;
  BattleSceneObject *object = state->object;
  int target_id = *(u16 *)&actor->actor.target_actor_id;
  BattleSceneObject *target = BattleSceneObject_GetById(target_id);
  BattleActor *enemy = BattleActor_GetById(target_id);
  Overlay21AttackConfig *config = state->config;
  int x_offset = config->impact_x;
  if (state->bits.secondary)
    x_offset += config->secondary_impact_x;
  int dx = object->x + x_offset - target->x;
  int dy = object->y + config->impact_y - target->y -
           (object->z + config->impact_z - target->z);
  BattlePosition position;
  ProjectImpact(&position, object, x_offset, config->impact_y,
                config->impact_z);
  if (BattleEnemy_GetStats(target_id)->trait_bits.unknown_trait_02) {
    BattleEffect_SetVariant(0);
    Overlay10Enemy_RestoreHitAnimation(*(u16 *)&actor->actor.target_actor_id);
    BattleSound_Play(53, 0, 0, 0);
    if (success) {
      int effect =
          actor->formation_index == 2 || actor->formation_index == 3 ? 21 : 24;
      BattleModelEffect_SpawnRelative(effect, object, 0, 0, 0, 0, 256);
    }
  } else {
    int power =
        state->bits.secondary ? config->secondary_power : config->primary_power;
    if (success) {
      int index = state->bits.secondary ? 2 : 0;
      HammerHitEffects *effects;
      int variant;
      if (BattleParty_RollHitBonus(actor, enemy)) {
        effects = &data_ov021_020c3fa0[index + 1];
        variant = 6;
      } else {
        effects = &data_ov021_020c3fa0[index];
        variant = 1;
      }
      BattleEffect_SetVariant((s16)variant);
      int damage =
          BattleDamage_CalculateAttack(actor, power, variant, enemy, 0, 0);
      BattleSpriteEffect_Spawn(effects->sprite, position.x, position.y,
                               position.z, 256);
      BattleModelEffect_Spawn(effects->model, 0, position.x, position.y,
                              position.z, 256);
      int effect =
          actor->formation_index == 2 || actor->formation_index == 3 ? 21 : 24;
      BattleModelEffect_SpawnRelative(effect, object, 0, 0, 0, 0, 256);
      BattleDamage_ApplyToEnemy(target, dx, dy, damage, effects->reaction,
                                effects->popup, 0);
      BattleSound_Play((u16)effects->sound, 0, 0, 0);
      if (config->status_chance &&
          BattleActor_CanReceiveStatus(BattleActor_GetById(target_id)))
        BattleStatus_TryApply(enemy, 2, *(s16 *)(gBattleContext + 300), 0,
                              config->status_chance);
      if (effects->screen_duration)
        BattleRumble_PlayRepeated(effects->screen_kind,
                                          effects->screen_duration,
                                          effects->screen_delay);
      else
        BattleRumble_PlayTimed(effects->screen_kind,
                                        effects->screen_duration,
                                        effects->screen_delay);
    } else {
      BattleEffect_SetVariant(-1);
      int damage = BattleDamage_CalculateAttack(actor, power, 0, enemy, 0, 0);
      BattleDamage_ApplyToEnemy(target, dx, dy, damage, 15, 7, 0);
      BattleSound_Play(52, 0, 0, 0);
    }
  }
  BattleFeedback_SpawnVariant(target, dx, dy, 1);
}

void Overlay21Attack_UpdateInput(Overlay21AttackState *state) {
  u16 pressed = *(u16 *)(gBattleContext + 0x104) & 0xC03;
  if (state->bits.secondary_phase == 1 && !state->secondary_timer) {
    Overlay21AttackConfig *config = state->config;
    state->bits.secondary_phase = 2;
    state->secondary_timer = config->phase_2_duration;
  }
  switch (state->bits.secondary_phase) {
  case 0:
    break;
  case 1:
    if (state->secondary_timer > 0)
      --state->secondary_timer;
    break;
  case 2:
    --state->secondary_timer;
    if (state->secondary_timer >= 0) {
      *(u16 *)(gBattleContext + 0xCB80) |= 2;
      if (pressed == data_ov002_020be704[state->actor->formation_index & 1]) {
        Overlay21Attack_BeginSecondary(state);
        return;
      }
    } else {
      state->bits.secondary_phase = 0;
    }
    break;
  }
  if (state->bits.result)
    return;
  switch (state->bits.primary_phase) {
  case 0:
    break;
  case 1:
    if (pressed)
      Overlay21Attack_EarlyInput(state);
    break;
  case 2:
    if (pressed) {
      if (state->bits.secondary)
        Overlay21Attack_MissSecondary(state);
      else
        Overlay21Attack_MissPrimary(state);
      Overlay21Attack_ClearSecondaryPhase(state);
    }
    break;
  case 3:
    --state->primary_timer;
    if (state->primary_timer >= 0) {
      if (state->bits.secondary)
        *(u16 *)(gBattleContext + 0xCB80) |= 4;
      else
        *(u16 *)(gBattleContext + 0xCB80) |= 1;
      if (pressed) {
        if (pressed ==
            data_ov002_020be704[(state->actor->formation_index & 1) + 2]) {
          if (state->bits.secondary)
            Overlay21Attack_HitSecondary(state);
          else
            Overlay21Attack_HitPrimary(state);
        } else {
          if (state->bits.secondary)
            Overlay21Attack_MissSecondary(state);
          else
            Overlay21Attack_MissPrimary(state);
        }
        Overlay21Attack_ClearSecondaryPhase(state);
      }
    } else {
      if (state->bits.secondary)
        Overlay21Attack_MissSecondary(state);
      else
        Overlay21Attack_MissPrimary(state);
      Overlay21Attack_ClearSecondaryPhase(state);
    }
    break;
  }
}
}
