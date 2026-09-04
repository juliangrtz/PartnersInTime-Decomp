#include <game/battle_ai.h>

void BattleTaskPool_Init(BattleTaskPool *pool, int count, u32 payload_size) {
    BattleAITask *task = (BattleAITask *)(pool + 1);
    u32 aligned_payload_size = (payload_size + 3) & ~3;

    pool->free = task;
    pool->active = 0;
    for (;;) {
        --count;
        if (count <= 0) {
            break;
        }
        task->next =
            (BattleAITask *)((u8 *)task + 0x0C + aligned_payload_size);
        task = task->next;
    }
    task->next = 0;
}

BattleAITask *BattleTaskPool_Allocate(BattleTaskPool *pool) {
    BattleAITask *task = pool->free;

    pool->free = task->next;
    task->owner_slot = 0;
    task->callback = 0;
    task->next = (BattleAITask *)pool;
    return task;
}

BattleAITask *BattleTaskList_Insert(BattleTaskPool *pool, BattleAITask *task) {
    if (task == 0) {
        task = BattleTaskPool_Allocate(pool);
    }

    if (task == 0) {
        return 0;
    }
    task->next = pool->active;
    pool->active = task;
    return task;
}

void BattleTask_Release(BattleAITask *task) {
    BattleAITask **owner_slot = task->owner_slot;

    if (owner_slot != 0) {
        BattleAITask *owned_task = *owner_slot;
        if (owned_task == task) {
            *owner_slot = 0;
        }
    }

    if (task->callback == 0) {
        BattleTaskPool *pool = (BattleTaskPool *)task->next;
        task->next = pool->free;
        pool->free = task;
    } else {
        task->callback = 0;
    }
}

BattleAITask *BattleTask_BindOwnerSlot(BattleAITask *task,
                                      BattleAITask **owner_slot) {
    BattleAITask *displaced;

    task->owner_slot = owner_slot;
    displaced = *owner_slot;
    if (displaced == task) {
        displaced = 0;
    }
    *owner_slot = task;
    return displaced;
}

BattleAITask *BattlePool_TakeNode(BattleTaskPool *pool) {
    BattleAITask *task = pool->free;
    pool->free = task->next;
    return task;
}

void BattlePool_ReturnNode(BattleTaskPool *pool, BattleAITask *task) {
    task->next = pool->free;
    pool->free = task;
}
