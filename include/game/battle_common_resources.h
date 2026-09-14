#ifndef PIT_GAME_BATTLE_COMMON_RESOURCES_H
#define PIT_GAME_BATTLE_COMMON_RESOURCES_H
#ifdef __cplusplus
extern "C" {
#endif
/* The entry task forwards its context; initialization uses the shared globals. */
void BattleCommonResources_Initialize(void *unused_context);
/* Advances resource slot 20 and prepares reserved scene objects 16 and 17. */
int BattleCommonResources_UpdateLoad(void);
void BattleCommonResources_Update(void *unused_context);
void BattleCommonResources_ReleaseGraphics(void *unused_context);
void BattleCommonResources_PrepareOam(void);
void BattleCommonResources_ResetOam(void);
void BattleCommonResources_OamReady(void);
#ifdef __cplusplus
}
#endif
#endif
