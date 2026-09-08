#ifndef PIT_OVERLAY020_ATTACK_INTERNAL_H
#define PIT_OVERLAY020_ATTACK_INTERNAL_H

#include <game/overlay020_attack_state.h>
#include <game/battle_scene.h>
#include <game/battle_effect.h>
#include <game/battle_context.h>
#include <game/heap.h>

typedef struct Overlay20AttackWork {
    u8 unknown0[17];
    s8 phase;
    u8 unknown18[22];
    Overlay20AttackState main;
    u8 unknown72[4];
    Overlay20AttackState support;
    u8 unknown108[4];
} Overlay20AttackWork;

typedef char Overlay20AttackWorkSize[sizeof(Overlay20AttackWork) == 112 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
extern Overlay20AttackConfig data_ov020_020c4fd0[];
extern Overlay20AttackWork *data_ov002_020c0710;
extern s8 data_ov020_020c4ee8[][2], data_ov020_020c4ee9[][2];
void func_0200940c(BattleModel *, s16);
int func_ov002_020716bc(void *, int, int, int);
int func_ov002_02071c34(void);
int func_ov002_020719a4(void);
void func_ov002_02071c44(void);
void func_ov002_020722ac(BattlePartyActor *, void (*)(BattlePartyActor *));
void func_0202cbd4(void *, int, unsigned);
void func_ov020_020c3eec(Overlay20AttackState *, Overlay20AttackState *);
void func_ov020_020c2efc(Overlay20AttackState *, int);
void func_ov020_020c326c(Overlay20AttackState *, Overlay20AttackState *);
#ifdef __cplusplus
}
#endif

#endif
