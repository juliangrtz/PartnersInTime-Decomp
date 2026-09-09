#ifndef PIT_OVERLAY017_PARTICIPANT_INTERNAL_H
#define PIT_OVERLAY017_PARTICIPANT_INTERNAL_H
#include <game/overlay017_participant.h>
#include "attack_state.h"
extern "C" {
#include <game/battle_actor.h>
#include <game/overlay010_motion.h>
extern void func_ov002_0206f1b8(BattleModel *);
extern void BattleSceneObject_SetAnimationMode01(BattleSceneObject *, int);
extern void func_0200940c(BattleModel *, int);
extern int func_ov002_02072004(BattleSceneObject *, int, int, int, int);
extern int func_ov017_020c5434(Overlay17Participant *, int, int);
extern void func_ov017_020c30e4(int, s8);
}
#endif
