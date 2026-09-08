#ifndef PIT_OVERLAY010_ATTACK_COMMON_INTERNAL_H
#define PIT_OVERLAY010_ATTACK_COMMON_INTERNAL_H
extern "C" {
#include <nitro.h>
#include <game/battle_actor.h>
#include <game/battle_damage.h>
#include <game/heap.h>
#include <game/item.h>
#include <game/overlay010_motion.h>
#include <game/save_data.h>
}
#include <game/battle_effect.h>

struct Overlay10ActionActor {
    BattlePartyActor party;
    u16 unknown_84, action_item;
};
struct Overlay10Transition {
    s16 first, second, phase, unknown;
};
struct Overlay10Work {
    int timer, timer2;
    u8 unknown_08[8];
    u8 flags;
    s8 phase;
    u8 unknown_12[6];
    Overlay10Transition transitions[2];
    Overlay10ActionActor *user;
    BattlePartyActor combined_actor;
    u8 unknown_b0[16];
    int *effect_clock;
    int effect_duration;
    u8 effect_shift;
    u8 unknown_c9[3];
    int score;
    u8 enemy_count, unknown_d1;
    u16 enemy_ids[6];
    s16 current_hp[6], position_z[6];
    u16 scale_steps[6], target;
    int parameter, parameter2;
    void (*callback)(Overlay10ActionActor *);
    s16 home[2][2], duration[2];
    u16 sound[2];
    s16 background_x, background_y, background_duration;
    u8 enabled : 1, prompt_animation : 5, background_phase : 2;
    u8 mirror : 1, animation : 7;
    u8 skip_prompt : 1, rating : 3, unknown_128 : 4;
    s8 unknown_129, prompt_x, prompt_y, offset_x, offset_y;
    u8 unknown_12e[2];
};
struct Overlay10TargetFlags {
    u8 uniform : 1, count_index : 2, unknown : 5;
};
struct Overlay10RatingFlags {
    u16 perfect : 1, unknown : 15;
};
struct Overlay10BadgeView {
    u8 unknown[1048];
    u8 badge;
};
struct Overlay10BattleFlags {
    u32 unknown_00 : 12, mode : 1, unknown_13 : 19;
};
typedef char Overlay10WorkSize[sizeof(Overlay10Work) == 304 ? 1 : -1];
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
void func_ov002_02071ac4(int, BattleSceneObject *, int, int, int);
int func_ov002_02071674(int);
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
