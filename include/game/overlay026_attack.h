#ifndef PARTNERS_IN_TIME_OVERLAY026_ATTACK_H
#define PARTNERS_IN_TIME_OVERLAY026_ATTACK_H

#include <game/battle_scene.h>
#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif

void Overlay26Attack_DestroyState(void *owner, int restore_party_objects);
int Overlay26Attack_IsStateIdle(void);
int Overlay26Attack_TrySetObjectAnimation(BattleSceneObject *object,
                                          int animation_id,
                                          int resource_id);
int Overlay26Attack_GetTrackedObjectState(u16 object_id);
int Overlay26Attack_GetActionState(const void *action);
int Overlay26Attack_IsResourceLoadPending(void);

#ifdef __cplusplus
}
#endif

#endif
