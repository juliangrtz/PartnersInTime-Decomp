#ifndef PIT_GAME_BATTLE_SOUND_H
#define PIT_GAME_BATTLE_SOUND_H
#ifdef __cplusplus
extern "C" {
#endif
/* A zero repeat count keeps a repeating sound active until explicitly stopped.
   A negative interval measures the initial effect's playback duration. */
int BattleSound_Play(int sound_id, int delay, int interval, int count);
void BattleSound_Stop(int slot);
#ifdef __cplusplus
}
#endif
#endif
