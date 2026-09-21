#ifndef PIT_GAME_FIELD_SCENE_TRANSITION_H
#define PIT_GAME_FIELD_SCENE_TRANSITION_H

/*
 * The requests the field makes to leave for another scene, and the update that
 * carries the pending one out.
 */
#include <game/field_system.h>
#ifdef __cplusplus
extern "C" {
#endif
int FieldSystem_UpdateSceneTransition(FieldSystem *system);
void FieldSystem_PreparePartyForBattle(FieldSystem *system);
void FieldSystem_RequestPause(FieldSystem *system, int option, int party_context,
                              int initial_section, u8 fade, u8 side);
void FieldSystem_RequestSave(FieldSystem *system, int party_context, int fade, int side);
void FieldSystem_RequestShop(FieldSystem *system, int shop_scene_id, int fade, int side);
void FieldSystem_RequestCredits(FieldSystem *system, int fade);
void FieldSystem_RequestGameOver(FieldSystem *system, int fade);
#ifdef __cplusplus
}
#endif
#endif
