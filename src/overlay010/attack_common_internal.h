#ifndef PIT_OVERLAY010_ATTACK_COMMON_INTERNAL_H
#define PIT_OVERLAY010_ATTACK_COMMON_INTERNAL_H
#include "attack_work.h"
extern "C" {
extern Overlay10Work *data_ov002_020c0710;
extern u8 *gBattleContext;
extern u8 data_ov010_020c2aa0[], data_ov010_020c2aa4[];
extern u16 data_ov010_020c2aac[];
extern s16 data_ov010_020c2ab4[][2];
extern s8 data_ov010_020c2a64[][2];
void func_0202cbd4(void *, int, u32);
int Overlay10Enemy_CollectIds(u16 *, int);
void Overlay10Enemy_Snapshot(Overlay10EnemyState *);
int Overlay10Enemy_IsSelectable(int);
int BattleDamage_CalculateAttack(BattlePartyActor *, int, int, int, int, int);
void func_ov002_020722ac(Overlay10ActionActor *, void (*)(Overlay10ActionActor *));
void func_ov010_020c1410(Overlay10ActionActor *);
void BattleFeedback_SpawnAtObject(int, BattleSceneObject *, int, int, int);
int BattleActor_IsDamageImmune(int);
void func_ov010_020c1024(s16 *, const GameGraphicsResource *, int);
extern u8 data_02049924[], data_02049918[];

void Overlay10Object_GetGraphicBounds(s16 *bounds, BattleSceneObject *object);
int Overlay10Party_HasBadgeTwo();
void Overlay10Attack_MoveBackground();
void Overlay10Attack_BeginEntry(Overlay10ActionActor *user, void (*callback)(Overlay10ActionActor *));
void Overlay10Attack_ShowRating(const s16 *thresholds, const s8 (*offsets)[2], int offset_index,
                                BattleSceneObject *object, int failed, const int *effects);
void Overlay10Enemy_PlayHitReaction(int actor_id, int effect_id);
void Overlay10Party_AverageStats(BattlePartyActor *output, int count);
int Overlay10Attack_CalculateDamage(BattlePartyActor *actor, int item, int argument, int target, int option);
int Overlay10Enemy_GetCenter(s16 *center);
void Overlay10Party_GetSparkPositions(BattlePosition *sprite, BattlePosition *model, BattlePartyActor *actor);
Overlay10Work *func_ov010_020c2978(Overlay10ActionActor *user, int size, int mode);
void func_ov010_020c1024(s16 *bounds, const GameGraphicsResource *resource, int frame);
void Overlay10Enemy_RestoreHitAnimation(int actor_id);
}
#endif
