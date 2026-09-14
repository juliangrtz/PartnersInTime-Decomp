#ifndef PIT_GAME_BATTLE_COMMON_RESOURCES_H
#define PIT_GAME_BATTLE_COMMON_RESOURCES_H
#ifdef __cplusplus
extern "C" {
#endif
/* The entry task forwards its context; initialization uses the shared globals. */
void BattleCommonResources_Initialize(void *unused_context);
/* Advances resource slot 20 and prepares reserved scene objects 16 and 17. */
int BattleCommonResources_UpdateLoad(void);
#ifdef __cplusplus
}
#endif
#endif
