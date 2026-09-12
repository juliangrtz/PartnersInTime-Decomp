#ifndef PIT_OVERLAY015_ACTOR_INTERNAL_H
#define PIT_OVERLAY015_ACTOR_INTERNAL_H

#include <game/overlay015_attack.h>
#include <game/battle_object.h>
#include <game/battle_feedback.h>
#include <game/battle_context.h>
#include <game/overlay010_motion.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <game/random.h>
/* Interleaved X/Y offsets; each actor's entry occupies two bytes. */
extern s8 data_ov015_020c5e60[][2];
extern s8 data_ov015_020c5e61[][2];
extern s8 data_ov015_020c5e68[][2];
extern s8 data_ov015_020c5e69[][2];

/* Formation-specific parameters have a six-byte record stride. */
extern s16 data_ov015_020c5f1c[][3];
extern s16 data_ov015_020c5ef8[][3];
extern s16 data_ov015_020c5f40[][3];
extern u16 data_ov015_020c5fdc[][2];
extern u16 data_ov015_020c5fde[][2];
extern u16 data_ov015_020c6054[][2];
extern u16 data_ov015_020c5fa0[];

int Overlay10Enemy_IsSelectable(u16 actor_id);

u16 func_02009224(BattleModel *model, s16 animation_id);
void func_0200940c(BattleModel *model, s16 speed_q8);
int BattleMotion_StartFollower(BattleSceneObject *object,
    BattleSceneObject *follower, int channel);
#ifdef __cplusplus
}
#endif

#endif
