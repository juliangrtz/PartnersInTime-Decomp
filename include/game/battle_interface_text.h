#ifndef PIT_GAME_BATTLE_INTERFACE_TEXT_H
#define PIT_GAME_BATTLE_INTERFACE_TEXT_H

#include <game/battle_context.h>
#include <game/battle_task_queue.h>

typedef struct BattleInterfaceNumberRequest {
    BattleInterfaceLayer *layer;
    int value;
    u32 position;
    u32 flags;
    u32 unknown_16;
} BattleInterfaceNumberRequest;

/* The upload queue passes a callback/argument prefix. */
typedef struct BattleInterfaceUploadTask {
    void *callback;
    BattleInterfaceLayer *layer;
} BattleInterfaceUploadTask;

typedef char BattleInterfaceNumberRequest_SizeCheck[sizeof(BattleInterfaceNumberRequest) == 20 ? 1 : -1];
typedef char BattleInterfaceUploadTask_PrefixSizeCheck[sizeof(BattleInterfaceUploadTask) == 8 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void BattleInterface_ProcessResourceTask(BattleQueuedTask *task);
int BattleInterface_UploadTextTask(void *argument);
BattleQueuedTask *BattleInterface_QueueNumber(BattleInterfaceLayer *layer, int value, int x, int y,
                                              int notify, int clear);
void BattleInterface_FormatNumberTask(BattleQueuedTask *task);
void BattleInterface_ProcessNumberTask(BattleQueuedTask *task);
#ifdef __cplusplus
}
#endif
#endif
