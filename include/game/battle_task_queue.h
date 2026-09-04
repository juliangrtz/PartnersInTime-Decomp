#ifndef PIT_GAME_BATTLE_TASK_QUEUE_H
#define PIT_GAME_BATTLE_TASK_QUEUE_H

#include <nitro.h>

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
