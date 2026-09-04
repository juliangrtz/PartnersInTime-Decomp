#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_interface_assets.h>

enum BattleInterfaceLayerOffset {
    BATTLE_INTERFACE_TASK_POOL_OFFSET = 0x8B44
};

typedef union BattleInterfaceResourceRequest {
    BattleAITask task_node;
    struct {
        BattleInterfaceLayer *layer;
        const void *resource;
        u32 modes;
        u32 flags;
    } fields;
} BattleInterfaceResourceRequest;

typedef char BattleInterfaceResourceRequest_SizeCheck[
    sizeof(BattleInterfaceResourceRequest) == 0x14 ? 1 : -1
];

void BattleInterface_ApplyResourceTask(BattleQueuedTask *task);

int BattleInterfaceLayer_QueueResource(
    BattleInterfaceLayer *layer, const void *resource, int horizontal_mode,
    int vertical_mode, int notify_on_complete, int clear_before_upload
) {
    BattleInterfaceResourceRequest *request;
    int flags;
    int clear_flag;
    int notify_flag;

    request = (BattleInterfaceResourceRequest *)BattlePool_TakeNode(
        (BattleTaskPool *)(gBattleContext + BATTLE_INTERFACE_TASK_POOL_OFFSET)
    );
    request->task_node.next = (BattleAITask *)layer;
    request->task_node.callback = (void (*)(BattleAITask *))resource;
    request->task_node.owner_slot = (BattleAITask **)(
        (horizontal_mode & 0xFFFF) | ((vertical_mode & 0xFFFF) << 16)
    );
    if (clear_before_upload != 0) {
        clear_flag = 2;
    } else {
        clear_flag = 0;
    }
    if (notify_on_complete != 0) {
        notify_flag = 1;
    } else {
        notify_flag = 0;
    }
    flags = notify_flag | clear_flag |
            (((layer->render.raw >> 28) & 0xFFFF) << 16);
    request->task_node.state = (BattleAIState *)flags;
    layer->flags.raw =
        (layer->flags.raw & ~0x0FFF) |
        ((u16)(8 * (layer->layout.bits.row_tiles_minus_one + 1)) & 0x0FFF);
    layer->flags.raw &= ~0x1000;
    return (int)BattleTaskQueue_Enqueue(
        BattleInterface_ApplyResourceTask, request
    );
}

int BattleInterfaceLayer_SetResource(
    BattleInterfaceLayer *layer, const void *resource, int horizontal_mode,
    int vertical_mode, int notify_on_complete, int clear_before_upload
) {
    int result;

    if (*(const void **)((u8 *)layer + 0x40) == resource) {
        return 0;
    }
    result = BattleInterfaceLayer_QueueResource(
        layer, resource, horizontal_mode, vertical_mode, notify_on_complete,
        clear_before_upload
    );
    *(const void **)((u8 *)layer + 0x40) = resource;
    *(u16 *)((u8 *)layer + 0x44) = 0;
    *(u8 *)((u8 *)layer + 0x46) = 0;
    ((BattleInterfaceLayerState *)layer)->transition_step = 64;
    *(u16 *)((u8 *)layer + 0x30) =
        (*(u16 *)((u8 *)layer + 0x30) & ~0x0FFF) |
        (*(u16 *)((u8 *)layer + 0x38) & 0x0FFF);
    return result;
}
