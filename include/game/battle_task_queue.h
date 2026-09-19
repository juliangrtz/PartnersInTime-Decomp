#ifndef PIT_GAME_BATTLE_TASK_QUEUE_H
#define PIT_GAME_BATTLE_TASK_QUEUE_H

#include <nitro.h>

/* Battle work that has to be spread over several frames - opening an archive,
   reading it, uploading the result - is written as a queued task instead of a
   blocking call. The scheduler calls `callback` every frame; the callback
   advances `state` itself and returns, so each step runs in its own frame and
   `timer` can hold it for a while. Enqueue returns the task so the caller can
   watch its state. */

typedef struct BattleQueuedTask BattleQueuedTask;
typedef void (*BattleQueuedTaskCallback)(BattleQueuedTask *task);

struct BattleQueuedTask {
    BattleQueuedTaskCallback callback;
    u16 state;
    u16 timer;
    void *argument;
};

typedef char BattleQueuedTask_SizeCheck[
    sizeof(BattleQueuedTask) == 0x0C ? 1 : -1
];

BattleQueuedTask *BattleTaskQueue_Enqueue(BattleQueuedTaskCallback callback,
                                          void *argument);

#endif
