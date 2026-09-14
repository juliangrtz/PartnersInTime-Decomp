#ifndef PIT_BATTLE_SCHEDULER_INTERNAL_H
#define PIT_BATTLE_SCHEDULER_INTERNAL_H

#include <nitro.h>
#include <game/battle_task_queue.h>

typedef struct BattleTransferTask BattleTransferTask;
typedef void (*BattleTransferCallback)(BattleTransferTask *);
struct BattleTransferTask {
    BattleTransferCallback callback;
    u32 argument_1;
    u32 argument_2;
    u32 argument_3;
};

typedef struct BattleSchedulerNode {
    struct BattleSchedulerNode *next;
    void (*update)(void);
    void (*vblank)(void);
    s16 priority;
    u16 padding;
} BattleSchedulerNode;

typedef union BattleSchedulerFlags {
    u16 raw;
    struct {
        u16 active_task : 1;
        u16 vblank_ready : 1;
        u16 inside_vblank : 1;
        u16 unknown : 13;
    } bits;
} BattleSchedulerFlags;

/* Overlay-2 scheduler allocation. Queue order follows the VRAM-bank
 * restoration boundary in the native VBlank consumer. */
typedef struct BattleScheduler {
    u8 archive_state[1088];
    u32 unknown_440;
    BattleSchedulerNode *first;
    void *buffer_a;
    void *buffer_b;
    void *irq_task;
    u32 texture_banks;
    u32 palette_banks;
    BattleQueuedTask active_task;
    BattleQueuedTask tasks[32];
    u16 task_head;
    u16 task_tail;
    BattleTransferTask before_mapping[32];
    BattleTransferTask deferred_before_mapping[32];
    u16 before_head;
    u16 before_tail;
    BattleTransferTask after_mapping[32];
    BattleTransferTask deferred_after_mapping[32];
    u16 after_head;
    u16 after_tail;
    u16 vblank_count;
    BattleSchedulerFlags flags;
    const u32 *requested_texture_banks;
    u8 padding_dfc[4];
} BattleScheduler;

typedef char BattleSchedulerSizeCheck[sizeof(BattleScheduler) == 3584 ? 1 : -1];
typedef char BattleTransferTaskSizeCheck[sizeof(BattleTransferTask) == 16 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
extern BattleScheduler *gBattleSystem;
void BattleScheduler_Idle(void);
void BattleScheduler_VBlank(void);
BattleSchedulerNode *BattleSchedulerNode_Insert(BattleSchedulerNode *node, int priority);
BattleSchedulerNode *BattleSchedulerNode_Unlink(BattleSchedulerNode *node);
BattleSchedulerNode *BattleSchedulerNode_UnlinkForCleanup(BattleSchedulerNode *node);
#ifdef __cplusplus
}
#endif

#endif
