#ifndef PIT_GAME_OVERLAY010_ATTACK_H
#define PIT_GAME_OVERLAY010_ATTACK_H

/* Shared Bros. Attack damage, rating and threshold lookup interfaces. */
#include <game/battle_actor.h>
#include <game/battle_scene.h>

#ifdef __cplusplus
extern "C" {
#endif
struct Overlay10ActionActor;
struct Overlay10Work;
struct Overlay10Work *func_ov010_020c2978(struct Overlay10ActionActor *user,
    int size, int mode);
void Overlay10Attack_BeginEntry(struct Overlay10ActionActor *user,
    void (*callback)(struct Overlay10ActionActor *));
int Overlay10Enemy_GetCenter(s16 *center);

int Overlay10Attack_CalculateDamage(BattlePartyActor *actor, int power,
    int hit_kind, BattleActor *target, int option);
void Overlay10Attack_ShowRating(const s16 *thresholds, const s8 (*offsets)[2],
    int offset_index, BattleSceneObject *object, int failed, const int *effects);
int Overlay10_SelectThresholdValue(const int *values, int input,
    const s16 *thresholds);
#ifdef __cplusplus
}
#endif
#endif
