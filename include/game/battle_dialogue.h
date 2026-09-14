#ifndef PIT_GAME_BATTLE_DIALOGUE_H
#define PIT_GAME_BATTLE_DIALOGUE_H

#ifdef __cplusplus
extern "C" {
#endif

/* A negative index tests or closes all four windows. Nonnegative indices
   are narrowed to signed halfwords by these three control functions. */
int BattleDialogue_IsAllocated(int index);
int BattleDialogue_IsOpen(int index);
void BattleDialogue_Close(int index);

/* Property 0 tests visual phase >= 2; property 1 returns baseline plus motion
   depth. This query uses a full-width slot index and requires its visual. */
int BattleDialogue_GetVisualProperty(int index, int property);
void BattleDialogue_Destroy(void);

#ifdef __cplusplus
}
#endif
#endif
