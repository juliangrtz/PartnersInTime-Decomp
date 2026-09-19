#ifndef PIT_GAME_BATTLE_CAPTURE_SURFACE_H
#define PIT_GAME_BATTLE_CAPTURE_SURFACE_H

/*
 * The task that copies the captured screen into the battle's surface buffer.
 */
#include <nitro.h>
typedef struct BattleCaptureCopyTask {
    int (*callback)(struct BattleCaptureCopyTask *task);
    const void *source;
    void *destination;
    u32 size;
} BattleCaptureCopyTask;


typedef char BattleCaptureCopyTask_SizeCheck[sizeof(BattleCaptureCopyTask) == 16 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int BattleCaptureSurface_CopyTask(BattleCaptureCopyTask *task);
#ifdef __cplusplus
}
#endif
#endif
