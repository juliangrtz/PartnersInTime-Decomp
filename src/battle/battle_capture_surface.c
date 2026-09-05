#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

enum BattleCaptureSurfaceOffset {
    BATTLE_CAPTURE_BUFFER_OFFSET = 0x37608,
    BATTLE_CAPTURE_VRAM_BASE_TABLE_OFFSET = 0x64,
    BATTLE_SYSTEM_TRANSFER_FLAGS_OFFSET = 0xDF6
};

enum BattleCaptureSurfaceConstant {
    BATTLE_CAPTURE_BUSY_FLAG = 1 << 30,
    BATTLE_CAPTURE_VRAM_ADDRESS_MASK = 0x1FFFF,
    BATTLE_CAPTURE_TILE_LIMIT = 32,
    BATTLE_CAPTURE_TILE_SIZE = 8,
    BATTLE_CAPTURE_TILE_BYTES = 64
};

typedef struct BattleCaptureCopyTask {
    int (*callback)(struct BattleCaptureCopyTask *task);
    const void *source;
    void *destination;
    u32 size;
} BattleCaptureCopyTask;

typedef struct BattleCaptureSurfaceState {
    u8 tile_x;
    u8 tile_y;
    u8 tile_width;
    u8 tile_height;
    u8 destination_stride;
} BattleCaptureSurfaceState;

extern u8 *gBattleSystem;

int func_0202cc58(const void *source, void *destination, u32 size);
int func_0202cd2c(const void *source, void *destination, u32 size);
const void *func_0203661c(void);
const u16 *func_020368dc(void);
void *func_ov002_020725a4(
    int (*callback)(), const void *argument_1,
    void *argument_2, u32 argument_3);
void func_ov002_020acd18(
    u32 *destination, const void *tile_data,
    u16 tile_entry, int destination_stride);

int BattleCaptureSurface_CopyTask(BattleCaptureCopyTask *task);
void BattleCaptureSurface_DecodeRowTask(BattleQueuedTask *task);
BattleQueuedTask *BattleCaptureSurface_QueueUpload(
    u32 vram_address, int object_data_id,
    int x, int y, int width, int height);

/* Metrowerks emits C functions in reverse source order. */
BattleQueuedTask *BattleCaptureSurface_QueueUpload(
    u32 vram_address, int object_data_id,
    int x, int y, int width, int height) {
    BattleCaptureSurfaceState *capture;
    BattleQueuedTask *task;
    BattleObjectDataLoadState *load_state;
    u32 destination_stride;
    u32 vram_offset;
    u32 vram_base;
    int leading_zeros;

    x = (x + 128) % 512;
    if (x < 0) {
        x += 512;
    }
    y = (y + 32) % 256;
    if (y < 0) {
        y += 256;
    }

    width = (width + (x + 7)) / BATTLE_CAPTURE_TILE_SIZE -
            x / BATTLE_CAPTURE_TILE_SIZE;
    x /= BATTLE_CAPTURE_TILE_SIZE;
    if (width > BATTLE_CAPTURE_TILE_LIMIT) {
        width = BATTLE_CAPTURE_TILE_LIMIT;
    }
    height = (height + (y + 7)) / BATTLE_CAPTURE_TILE_SIZE -
             y / BATTLE_CAPTURE_TILE_SIZE;
    y /= BATTLE_CAPTURE_TILE_SIZE;
    if (height > BATTLE_CAPTURE_TILE_LIMIT) {
        height = BATTLE_CAPTURE_TILE_LIMIT;
    }

    leading_zeros = width;
    asm {
        clz leading_zeros, leading_zeros
    }
    destination_stride = 0x80000000u >> leading_zeros;
    if ((width & (destination_stride - 1)) != 0) {
        destination_stride *= 2;
    }

    task = BattleTaskQueue_Enqueue(
        BattleCaptureSurface_DecodeRowTask, 0);
    task->state = object_data_id;
    load_state = BattleObjectData_GetLoadState(object_data_id);
    load_state->flags.raw |= BATTLE_CAPTURE_BUSY_FLAG;
    vram_offset = vram_address & BATTLE_CAPTURE_VRAM_ADDRESS_MASK;
    vram_base =
        *(u32 *)(gBattleContext + BATTLE_CAPTURE_VRAM_BASE_TABLE_OFFSET +
                 (vram_address >> 17) * 4);
    load_state->data = (u8 *)(vram_offset + vram_base);
    load_state->component_04 =
        gBattleContext + BATTLE_CAPTURE_BUFFER_OFFSET;
    load_state->component_14 = load_state->component_04;

    capture = (BattleCaptureSurfaceState *)&load_state->allocation_size;
    capture->tile_x = x;
    capture->tile_y = y;
    capture->tile_width = width;
    capture->tile_height = height;
    capture->destination_stride = destination_stride;
    load_state->texture_variant = 0;
    return task;
}

void BattleCaptureSurface_DecodeRowTask(BattleQueuedTask *task) {
    BattleObjectDataLoadState *load_state;
    u32 *destination;
    BattleCaptureSurfaceState *capture;
    int row;
    int tile_width;
    int tile_x;
    int tile_y;
    int destination_stride;
    int row_group;
    int row_offset;
    const void *tile_data;
    const u16 *tile_map;

    load_state = BattleObjectData_GetLoadState(task->state);
    capture = (BattleCaptureSurfaceState *)&load_state->allocation_size;
    row = load_state->texture_variant;
    tile_width = capture->tile_width;
    destination = (u32 *)load_state->component_04;
    tile_x = capture->tile_x;
    tile_y = capture->tile_y;
    destination_stride = capture->destination_stride;
    row_group = row / tile_width;

    if (row_group == capture->tile_height) {
        func_ov002_020725a4(
            (int (*)())BattleCaptureSurface_CopyTask,
            load_state->component_14, load_state->data,
            (row_group * destination_stride) << 6);
        load_state->flags.raw &= ~BATTLE_CAPTURE_BUSY_FLAG;
        *(u16 *)(gBattleSystem + BATTLE_SYSTEM_TRANSFER_FLAGS_OFFSET) =
            (*(u16 *)(gBattleSystem +
                       BATTLE_SYSTEM_TRANSFER_FLAGS_OFFSET) & ~1) | 1;
        task->callback = 0;
        return;
    }

    tile_y += row_group;
    row_offset = row % tile_width;
    if (row_offset == 0 && row != 0) {
        int copied_size = (row_group * destination_stride) << 6;

        destination += 4 * destination_stride - 2 * tile_width;
        if (copied_size > 0x2000) {
            int copy_words = copied_size / 4;

            func_ov002_020725a4(
                (int (*)())BattleCaptureSurface_CopyTask,
                load_state->component_14, load_state->data, copied_size);
            load_state->data += copy_words * 4;
            load_state->component_14 =
                (u8 *)load_state->component_14 + copy_words * 4;
            capture->tile_y += row_group;
            capture->tile_height -= row_group;
            load_state->texture_variant = 0;
            *(u16 *)(gBattleSystem + BATTLE_SYSTEM_TRANSFER_FLAGS_OFFSET) =
                (*(u16 *)(gBattleSystem +
                           BATTLE_SYSTEM_TRANSFER_FLAGS_OFFSET) & ~1) | 1;
        }
    }

    tile_x += row_offset;
    tile_data = func_0203661c();
    tile_map = func_020368dc();
    func_ov002_020acd18(
        destination, tile_data,
        tile_map[((tile_x & 0x1F) + ((tile_y & 0x1F) << 5) +
                  ((tile_x & 0x20) << 5))],
        destination_stride * 2);
    load_state->component_04 = destination + 2;
    ++load_state->texture_variant;
}

int BattleCaptureSurface_CopyTask(BattleCaptureCopyTask *task) {
    u32 size = task->size;
    void *destination = task->destination;
    const void *source = task->source;

    if (source < destination) {
        return func_0202cd2c(source, destination, size);
    }
    return func_0202cc58(source, destination, size);
}
