#ifndef PIT_GAME_SCENE_SOUND_H
#define PIT_GAME_SCENE_SOUND_H
#include <nitro.h>
typedef struct SceneSoundTask SceneSoundTask;
struct SceneSoundTask {
    u8 unknown_00[12];
    void (*callback)(SceneSoundTask *);
    u8 unknown_10[24];
    u16 slot;
    s16 sequence;
    s16 timer;
    s16 interval;
    struct {
        u16 remaining : 14, handle : 2;
    } state;
    u8 unknown_32[22];
};
typedef char SceneSoundTask_SizeCheck[sizeof(SceneSoundTask) == 72 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int SceneSound_PlayRepeated(int sequence, int delay, int interval, int count);
void SceneSoundTask_PlayOnce(SceneSoundTask *task);
void SceneSoundTask_BeginMeasuredPlayback(SceneSoundTask *task);
void SceneSoundTask_MeasureInterval(SceneSoundTask *task);
void SceneSoundTask_UpdatePeriodic(SceneSoundTask *task);
void SceneSound_StopTask(int slot);
#ifdef __cplusplus
}
#endif
#endif
