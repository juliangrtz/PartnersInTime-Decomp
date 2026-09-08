#include <game/battle_ai.h>
#include <game/battle_interface_text.h>
#include <game/text.h>

enum { BATTLE_INTERFACE_TASK_POOL_OFFSET = 35652, BATTLE_INTERFACE_UPLOAD_FLAGS_OFFSET = 3574 };

extern u8 *gBattleSystem;
extern u8 data_ov002_020c0c44[];
void func_0202cbd4(void *destination, int value, u32 length);
void func_0202cf04(void *source, u32 destination, int row_bytes, int height);
void *func_ov002_020725a4(int (*callback)(void *), void *argument, int a, int b);

void BattleInterface_ProcessResourceTask(BattleQueuedTask *task)
{
    BattleInterfaceLayer *layer = (BattleInterfaceLayer *)task->argument;
    GameText *text = (GameText *)layer;
    int i;
    if (layer->flags.bits.clear_before_upload) {
        layer->flags.bits.clear_before_upload = 0;
        func_0202cbd4(text->destination, 0, layer->allocation_size);
        return;
    }
    for (i = 0;; i++) {
        if (i >= 4)
            return;
        if (layer->flags.bits.row_bytes + 8 <= (int)text->cursor.bits.x) {
            layer->flags.bits.state_15 = 1;
            goto complete;
        }
        if (!*text->text)
            goto complete;
        GameText_Next(text, 0, 0);
    }
/* A shared exit preserves the native return at the four-token limit. */
complete:
    if (!layer->flags.bits.state_15)
        text->text = 0;
    if (layer->flags.bits.notify_on_complete) {
        func_ov002_020725a4(BattleInterface_UploadTextTask, layer, 0, 0);
        *(u16 *)(gBattleSystem + BATTLE_INTERFACE_UPLOAD_FLAGS_OFFSET) =
            (*(u16 *)(gBattleSystem + BATTLE_INTERFACE_UPLOAD_FLAGS_OFFSET) & ~1) | 1;
    }
    layer->flags.bits.resource_ready = 1;
    task->callback = 0;
}

int BattleInterface_UploadTextTask(void *argument)
{
    BattleInterfaceLayer *layer = ((BattleInterfaceUploadTask *)argument)->layer;
    int columns = layer->layout.bits.row_tiles_minus_one + 1;
    u32 offset = layer->vram_offset;
    int rows = layer->allocation_size / (32 * columns);
    int flags;
    u32 *banks = (u32 *)(gBattleContext + 100);
    func_0202cf04(layer->pixel_buffer, (offset & 0x1ffff) + banks[offset >> 17], 4 * columns, 8 * rows);
    flags = layer->flags.raw & ~0x2000;
    layer->flags.raw = flags;
    return flags;
}

BattleQueuedTask *BattleInterface_QueueNumber(BattleInterfaceLayer *layer, int value, int x, int y,
                                              int notify, int clear)
{
    BattleInterfaceNumberRequest *request = (BattleInterfaceNumberRequest *)BattlePool_TakeNode(
        (BattleTaskPool *)(gBattleContext + BATTLE_INTERFACE_TASK_POOL_OFFSET));
    int clear_flag;
    int notify_flag;
    request->layer = layer;
    request->value = value;
    request->position = (x & 0xffff) | ((y & 0xffff) << 16);
    if (clear)
        clear_flag = 2;
    else
        clear_flag = 0;
    if (notify)
        notify_flag = 1;
    else
        notify_flag = 0;
    request->flags = notify_flag | clear_flag | (((layer->render.raw >> 28) & 0xffff) << 16);
    layer->flags.bits.row_bytes = 8 * (layer->layout.bits.row_tiles_minus_one + 1);
    layer->flags.bits.resource_ready = 0;
    return BattleTaskQueue_Enqueue(BattleInterface_FormatNumberTask, request);
}

void BattleInterface_FormatNumberTask(BattleQueuedTask *task)
{
    BattleInterfaceNumberRequest *request = (BattleInterfaceNumberRequest *)task->argument;
    BattleInterfaceLayer *layer = request->layer;
    GameText *text = (GameText *)layer;
    int value = request->value;
    s16 x = request->position, y = request->position >> 16;
    s16 notify = request->flags & 1, clear = request->flags & 2;
    s16 color = request->flags >> 16;
    int length;
    BattlePool_ReturnNode((BattleTaskPool *)(gBattleContext + BATTLE_INTERFACE_TASK_POOL_OFFSET),
                          (BattleAITask *)request);
    task->argument = layer;
    length = 0;
    /* Negative sentinels display dashes; ordinary digits are drawn right to left. */
    if (value < 0) {
        do {
            data_ov002_020c0c44[length++] = '-';
            value++;
        } while (value < 0);
        data_ov002_020c0c44[length] = 0;
    } else {
        do {
            data_ov002_020c0c44[length++] = value % 10 + '0';
            value /= 10;
        } while (value);
        data_ov002_020c0c44[length] = 0;
    }
    GameText_Init(text, text->fonts, text->destination, data_ov002_020c0c44, x, y, text->cursor.bits.spacing,
                  text->cursor.bits.leading, color, text->style.bits.space_width, text->style.bits.tile_pitch,
                  0, 255, 0);
    layer->flags.bits.notify_on_complete = (u16)(notify != 0);
    layer->flags.bits.clear_before_upload = (u16)(clear != 0);
    task->callback = BattleInterface_ProcessNumberTask;
}

void BattleInterface_ProcessNumberTask(BattleQueuedTask *task)
{
    BattleInterfaceLayer *layer = (BattleInterfaceLayer *)task->argument;
    GameText *text = (GameText *)layer;
    int i;
    if (layer->flags.bits.clear_before_upload) {
        layer->flags.bits.clear_before_upload = 0;
        func_0202cbd4(text->destination, 0, layer->allocation_size);
        return;
    }
    for (i = 0;; i++) {
        int x;
        if (i >= 4)
            return;
        if (!*text->text || (*text->text == 255 && !text->text[1]))
            goto complete;
        x = text->cursor.bits.x;
        GameText_Next(text, 0, 0);
        text->cursor.bits.x = x - 8;
    }
/* A shared exit preserves the native return at the four-token limit. */
complete:
    if (layer->flags.bits.notify_on_complete) {
        func_ov002_020725a4(BattleInterface_UploadTextTask, layer, 0, 0);
        *(u16 *)(gBattleSystem + BATTLE_INTERFACE_UPLOAD_FLAGS_OFFSET) =
            (*(u16 *)(gBattleSystem + BATTLE_INTERFACE_UPLOAD_FLAGS_OFFSET) & ~1) | 1;
    }
    layer->flags.bits.resource_ready = 1;
    text->text = 0;
    task->callback = 0;
}
