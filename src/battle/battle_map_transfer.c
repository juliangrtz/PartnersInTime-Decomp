#include <game/battle_archive.h>
#include <game/archive_io.h>
#include <game/battle_scene.h>
#include <game/matrix_animation.h>
#include <game/palette_animation.h>

extern void *gBattleSystem;
extern const void *data_ov002_020bebd0;
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
u8 *func_020365fc(void), *func_0203661c(void);
u16 *func_020368dc(void), *func_02036830(void), *func_02036724(void);
extern const BattleMapChannelRecord data_ov002_020befb0[];
u16 *func_020368bc(void), *func_020367b0(void), *func_020366a4(void);
void func_ov002_020aaac8(int, MtxFx44 *, GameMatrixAnimationTrack *);
void GX_ResetBankForTex(void);
void func_02035c00(int);
void func_02035a40(int);
void *func_02036650(void);
void *func_02036910(void);
void func_0202cbd4(void *, int, u32);
void func_ov002_02072508(void (*)(void), int, int, int);
enum {
    BATTLE_MAP_ARCHIVE_BUFFER_OFFSET = 57704,
    BATTLE_MAP_SUB_OBJECT_OFFSET = 25900,
    BATTLE_MAP_MAIN_OBJECT_OFFSET = 25896,
    BATTLE_MAP_CHANNELS_OFFSET = 52136,
    BATTLE_MAP_SCROLL_OFFSETS_OFFSET = 52152,
    BATTLE_MAP_MAIN_PALETTE_CURSOR_OFFSET = 52172,
    BATTLE_MAP_SUB_PALETTE_CURSOR_OFFSET = 52176,
    BATTLE_MAP_PALETTE_COMMANDS_OFFSET = 52184,
    BATTLE_MAP_MAIN_PALETTE_OFFSET = 399368,
    BATTLE_MAP_SUB_PALETTE_OFFSET = 400392,
    BATTLE_MAP_RESOURCE_OFFSET = 52204,
    BATTLE_MAP_WORK_BUFFER_OFFSET = 226824,
    BATTLE_MAP_PALETTE_LOCATION_OFFSET = 52100,
    BATTLE_MAP_BUFFER_TABLE_OFFSET = 116,
    BATTLE_MAP_RESOURCE_COUNT = 792,
    BATTLE_MAP_SECTION_COUNT = 16,
    BATTLE_MAP_TILEMAP_ROWS = 32,
    BATTLE_MAP_TILEMAP_WIDTH = 64,
    BATTLE_MAP_SCREEN_WIDTH = 32,
    BATTLE_MAP_SCREEN_TILES = 1024,
    BATTLE_MAP_ANIMATION_TRACKS_OFFSET = 52192,
    BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET = 52200,
    BATTLE_MAP_MATRIX_ANIMATION_OFFSET = 52212,
    BATTLE_GRAPHICS_LOAD_FLAGS_OFFSET = 3574
};
static inline BattleContext *Context(void)
{
    return (BattleContext *)gBattleContext;
}
static inline BattleSceneResource *MapResource(void)
{
    return *(BattleSceneResource **)(gBattleContext + BATTLE_MAP_RESOURCE_OFFSET);
}

static inline void Copy(const void *source, void *destination, u32 size)
{
    if (source < destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
}
static inline void CopyTilemapRows(u8 **buffers, int entry, u16 *(*get_map)(void))
{
    int row;
    for (row = 0; row < BATTLE_MAP_TILEMAP_ROWS; ++row) {
        Copy((u16 *)buffers[entry] + BATTLE_MAP_TILEMAP_WIDTH * row,
             get_map() + BATTLE_MAP_SCREEN_WIDTH * row, 64);
        Copy((u16 *)buffers[entry] + BATTLE_MAP_SCREEN_WIDTH * (2 * row + 1),
             get_map() + BATTLE_MAP_SCREEN_WIDTH * row + BATTLE_MAP_SCREEN_TILES, 64);
    }
}
static inline GameMatrixAnimationTrack **MapAnimationTrack(int index)
{
    return (GameMatrixAnimationTrack **)(gBattleContext + BATTLE_MAP_ANIMATION_TRACKS_OFFSET + index * 4);
}
static inline void StartMapAnimation(int index, const s16 *commands)
{
    GameMatrixAnimationTrack *track = *MapAnimationTrack(index);
    if (track)
        track->commands = commands;
    else {
        GameMatrixAnimationTrack *created = GameMatrixAnimation_Start(
            *(GameMatrixAnimation **)(gBattleContext + BATTLE_MAP_MATRIX_ANIMATION_OFFSET), commands,
            func_ov002_020aaac8, 4096);
        *MapAnimationTrack(index) = created;
    }
    (*MapAnimationTrack(index))->flags.raw |= 4;
    (*MapAnimationTrack(index))->flags.bits.loop = 1;
}

void BattleMap_ReadEntry(int entry, u8 **destinations, u32 *sizes)
{
    Context()->asset_read.entry_index = entry;
    Context()->asset_read.read_size = sizes[entry];
    if (Context()->asset_read.read_size) {
        Context()->asset_read.destination = destinations[entry];
        BattleArchive_ReadAsync(gBattleSystem, &Context()->asset_read, 0,
                                Context()->asset_read.archive_cursor, Context()->asset_read.descriptor, 0);
    }
}

void BattleMap_ReadCompressedEntry(int entry, u8 **destinations, u32 *sizes)
{
    u32 size;
    Context()->asset_read.entry_index = entry;
    size = (ArchiveIO_PrepareCompressedRead(gBattleSystem, (ArchiveCompressedRequest *)&Context()->asset_read,
                                            (u32 *)Context()->asset_read.archive_cursor,
                                            Context()->asset_read.descriptor) +
            3) &
           ~3;
    Context()->asset_read.read_size = size;
    sizes[entry] = Context()->asset_read.read_size;
    ArchiveIO_QueueCompressedRead(gBattleSystem, (ArchiveCompressedRequest *)&Context()->asset_read,
                                  destinations[entry]);
}

void BattleMap_SavePalette(void)
{
    u32 location = *(u32 *)(gBattleContext + BATTLE_MAP_PALETTE_LOCATION_OFFSET);
    u8 *destination =
        (u8 *)(*(u32 *)(gBattleContext + BATTLE_MAP_BUFFER_TABLE_OFFSET + 4 * (location >> 14)) +
               (location & 0x3fff));
    if (destination > (u8 *)0x05000000)
        func_0202cd2c((void *)0x05000000, destination, 512);
    else
        func_0202cc58((void *)0x05000000, destination, 512);
}

BattleQueuedTask *BattleMap_QueueSubDisplayLoad(void)
{
    MapResource()->flags.raw |= 0x40000000;
    Context()->runtime.flags.raw |= 0x40;
    func_ov002_02072508(BattleMap_ConfigureDisplays, 0, 0, 0);
    return BattleTaskQueue_Enqueue(BattleMap_BeginSubDisplayTask, 0);
}

void BattleMap_BeginSubDisplayTask(BattleQueuedTask *task)
{
    *(u16 *)((u8 *)gBattleSystem + BATTLE_GRAPHICS_LOAD_FLAGS_OFFSET) =
        (*(u16 *)((u8 *)gBattleSystem + BATTLE_GRAPHICS_LOAD_FLAGS_OFFSET) & ~1) | 1;
    task->callback = BattleMap_ReadSubDisplayTask;
}

void BattleMap_ConfigureDisplays(void)
{
    GX_ResetBankForTex();
    func_02035c00(3);
    func_02035a40(4);
    *(volatile u32 *)0x04000000 = (*(volatile u32 *)0x04000000 & ~0x1f00) | 0xf00;
    *(volatile u32 *)0x04001000 = (*(volatile u32 *)0x04001000 & ~0x1f00) | 0x1f00;
    func_0202cbd4(func_02036650(), 0, 32);
    func_0202cbd4(func_02036910(), 0, 2048);
    Context()->runtime.flags.raw &= ~0x40;
}

void BattleMap_ReadSubDisplayTask(BattleQueuedTask *task)
{
    u8 **destinations;
    u32 *offsets = (u32 *)MapResource()->data;
    destinations = (u8 **)(offsets + BATTLE_MAP_SECTION_COUNT);
    destinations[8] = func_020365fc();
    destinations[10] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[11] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[12] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    Context()->asset_read.archive_cursor = (u8 *)offsets;
    Context()->asset_read.descriptor = data_ov002_020bebd0;
    BattleMap_ReadCompressedEntry(8, destinations, offsets + 2 * BATTLE_MAP_SECTION_COUNT);
    task->callback = BattleMap_UpdateSubDisplayTask;
}

void BattleMap_UpdateSubDisplayTask(BattleQueuedTask *task)
{
    BattleSceneResource *resource = MapResource();
    u8 **buffers = (u8 **)(resource->data + BATTLE_MAP_SECTION_COUNT * sizeof(u32));
    u32 *sizes = (u32 *)(resource->data + 2 * BATTLE_MAP_SECTION_COUNT * sizeof(u32));
    switch (Context()->asset_read.entry_index) {
    case 8:
        BattleMap_ReadEntry(10, buffers, sizes);
        return;
    case 10:
        CopyTilemapRows(buffers, 10, func_020368bc);
        BattleMap_ReadEntry(11, buffers, sizes);
        return;
    case 11:
        if (sizes[11])
            CopyTilemapRows(buffers, 11, func_020367b0);
        else
            func_0202cbd4(func_020367b0(), 0, 4096);
        BattleMap_ReadEntry(12, buffers, sizes);
        return;
    case 12:
        if (sizes[12])
            CopyTilemapRows(buffers, 12, func_020366a4);
        else
            func_0202cbd4(func_020366a4(), 0, 4096);
        if (*(const s16 **)(gBattleContext + BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET))
            StartMapAnimation(1, *(const s16 **)(gBattleContext + BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET));
        task->callback = 0;
        resource->flags.raw &= ~0x40000000;
        return;
    }
}

BattleQueuedTask *BattleMap_QueueReload(BattleSceneResource *resource, int index)
{
    BattleQueuedTask *task;
    if (index < 0)
        return 0;
    if (index >= BATTLE_MAP_RESOURCE_COUNT)
        return 0;
    task = BattleTaskQueue_Enqueue(BattleMap_OpenReloadArchiveTask, 0);
    resource->resource_id = index;
    resource->flags.raw |= 0x40000000;
    *(BattleSceneResource **)(gBattleContext + BATTLE_MAP_RESOURCE_OFFSET) = resource;
    return task;
}
void BattleMap_OpenReloadArchiveTask(BattleQueuedTask *task)
{
    int size;
    Context()->asset_read.archive_cursor = gBattleContext + BATTLE_MAP_ARCHIVE_BUFFER_OFFSET;
    Context()->asset_read.descriptor = data_ov002_020bebd0;
    size = BattleArchive_GetDescriptorSize(gBattleSystem, Context()->asset_read.descriptor);
    BattleArchive_Open(gBattleSystem, gBattleContext + BATTLE_MAP_ARCHIVE_BUFFER_OFFSET, size,
                       Context()->asset_read.descriptor);
    task->callback = BattleMap_PrepareReloadTask;
}
void BattleMap_ClearDisplays(void)
{
    func_0202cbd4(func_0203661c(), 0, 64);
    func_0202cbd4(func_020368bc(), 0, 64);
    func_0202cbd4(func_020368dc(), 0, 4096);
    func_0202cbd4(func_02036830(), 0, 4096);
    func_0202cbd4(func_02036724(), 0, 4096);
    func_0202cbd4(func_020368bc(), 0, 4096);
    func_0202cbd4(func_020367b0(), 0, 4096);
    func_0202cbd4(func_020366a4(), 0, 4096);
}
void BattleMap_PrepareReloadTask(BattleQueuedTask *task)
{
    BattleSceneResource *resource;
    u8 **destinations;
    u32 *sizes;
    int index;
    u32 *offsets;
    int i;
    u32 next;
    const BattleMapChannelRecord *config;
    resource = *(BattleSceneResource **)(gBattleContext + BATTLE_MAP_RESOURCE_OFFSET);
    index = resource->resource_id;
    offsets = (u32 *)resource->data;
    destinations = (u8 **)(resource->data + BATTLE_MAP_SECTION_COUNT * sizeof(u32));
    sizes = (u32 *)(resource->data + 2 * BATTLE_MAP_SECTION_COUNT * sizeof(u32));
    Copy((u32 *)Context()->asset_read.archive_cursor + index, offsets, 64);
    i = 16;
    next = ((u32 *)Context()->asset_read.archive_cursor)[index + 16];
    do {
        --i;
        if (next != offsets[i]) {
            sizes[i] = next - offsets[i];
            next = offsets[i];
        } else
            sizes[i] = 0;
    } while (i > 0);
    destinations[0] = func_0203661c();
    destinations[8] = func_020365fc();
    destinations[1] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[2] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[3] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[4] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[9] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[10] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[11] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    destinations[12] = gBattleContext + BATTLE_MAP_WORK_BUFFER_OFFSET;
    /* Each resource group supplies one eight-byte channel record per display. */
    config = &data_ov002_020befb0[2 * (index / 16)];
    destinations[6] = resource->data + 3 * BATTLE_MAP_SECTION_COUNT * sizeof(u32);
    destinations[7] = destinations[6] + sizes[6];
    destinations[14] = destinations[7] + sizes[7];
    destinations[15] = destinations[14] + sizes[14];
    /* Animation pairs share one asynchronous read per display. */
    offsets[7] = sizes[7] + (offsets[6] + sizes[6]);
    offsets[15] = sizes[15] + (offsets[14] + sizes[14]);
    (*(BattleSceneObject **)(gBattleContext + BATTLE_MAP_SUB_OBJECT_OFFSET))->resource = resource;
    (*(BattleSceneObject **)(gBattleContext + BATTLE_MAP_MAIN_OBJECT_OFFSET))->resource =
        (*(BattleSceneObject **)(gBattleContext + BATTLE_MAP_SUB_OBJECT_OFFSET))->resource;
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 0] = config[0].fields[0];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 1] = config[0].fields[1];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 2] = config[0].fields[2];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 3] = config[0].fields[3];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 4] = config[1].fields[0];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 5] = config[1].fields[1];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 6] = config[1].fields[2];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 7] = config[1].fields[3];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 8] = config[0].fields[4];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 9] = config[0].fields[5];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 10] = config[0].fields[6];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 11] = config[0].fields[7];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 12] = config[1].fields[4];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 13] = config[1].fields[5];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 14] = config[1].fields[6];
    gBattleContext[BATTLE_MAP_CHANNELS_OFFSET + 15] = config[1].fields[7];
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 2) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 4) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 6) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 8) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 10) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 12) = 0;
    *(u16 *)(gBattleContext + BATTLE_MAP_SCROLL_OFFSETS_OFFSET + 14) = 0;
    if (!sizes[0]) {
        task->callback = 0;
        resource->flags.raw &= ~0x40000000;
        return;
    }
    {
        *(u32 *)(gBattleContext + BATTLE_MAP_PALETTE_COMMANDS_OFFSET) = 0;
        *(const s16 **)(gBattleContext + BATTLE_MAP_PALETTE_COMMANDS_OFFSET + 4) = 0;
        if (*MapAnimationTrack(0)) {
            (*MapAnimationTrack(0))->commands = 0;
            *MapAnimationTrack(0) = 0;
        }
        *(const s16 **)(gBattleContext + BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET) = 0;
        if (*MapAnimationTrack(1)) {
            (*MapAnimationTrack(1))->commands = 0;
            *MapAnimationTrack(1) = 0;
        }
        func_ov002_02072508(BattleMap_ClearDisplays, 0, 0, 0);
        *(u16 *)((u8 *)gBattleSystem + BATTLE_GRAPHICS_LOAD_FLAGS_OFFSET) =
            (*(u16 *)((u8 *)gBattleSystem + BATTLE_GRAPHICS_LOAD_FLAGS_OFFSET) & ~1) | 1;
        task->callback = BattleMap_ReadReloadTask;
    }
}
void BattleMap_ReadReloadTask(BattleQueuedTask *task)
{
    u32 *offsets = (u32 *)MapResource()->data;
    Context()->asset_read.archive_cursor = (u8 *)offsets;
    BattleMap_ReadCompressedEntry(0, (u8 **)(offsets + BATTLE_MAP_SECTION_COUNT),
                                  offsets + 2 * BATTLE_MAP_SECTION_COUNT);
    task->callback = BattleMap_UpdateReloadTask;
}

static inline void ReadMapAnimationPair(int entry, u8 **buffers, u32 *sizes)
{
    Context()->asset_read.entry_index = entry;
    Context()->asset_read.read_size = sizes[entry] + sizes[entry + 1];
    if (Context()->asset_read.read_size) {
        Context()->asset_read.destination = buffers[entry];
        BattleArchive_ReadAsync(gBattleSystem, &Context()->asset_read, 0,
                                Context()->asset_read.archive_cursor, Context()->asset_read.descriptor, 0);
    }
}

void BattleMap_UpdateReloadTask(BattleQueuedTask *task)
{
    BattleSceneResource *resource = *(BattleSceneResource **)(gBattleContext + BATTLE_MAP_RESOURCE_OFFSET);
    u8 **buffers = (u8 **)(resource->data + 64);
    u32 *sizes = (u32 *)(resource->data + 128);
    switch (Context()->asset_read.entry_index) {
    case 16:
        BattleMap_ReadCompressedEntry(0, buffers, sizes);
        return;
    case 0:
        BattleMap_ReadEntry(1, buffers, sizes);
        return;
    case 1:
        *(u32 *)(gBattleContext + BATTLE_MAP_MAIN_PALETTE_CURSOR_OFFSET) = 0;
        Copy(buffers[1], gBattleContext + BATTLE_MAP_MAIN_PALETTE_OFFSET, 512);
        BattleMap_ReadEntry(2, buffers, sizes);
        return;
    case 2:
        CopyTilemapRows(buffers, 2, func_020368dc);
        BattleMap_ReadEntry(3, buffers, sizes);
        return;
    case 3:
        if (sizes[3])
            CopyTilemapRows(buffers, 3, func_02036830);
        BattleMap_ReadEntry(4, buffers, sizes);
        return;
    case 4:
        if (sizes[4])
            CopyTilemapRows(buffers, 4, func_02036724);
        ReadMapAnimationPair(6, buffers, sizes);
        return;
    case 6:
        if (sizes[6])
            *(const s16 **)(gBattleContext + BATTLE_MAP_PALETTE_COMMANDS_OFFSET) =
                GameAnimationTable_GetComponent((const s16 *)buffers[6], -1, -1);
        if (sizes[7]) {
            /* Read the commands in each branch, after testing track ownership. */
            GameMatrixAnimationTrack *track = *MapAnimationTrack(0);
            if (track)
                track->commands = (const s16 *)buffers[7];
            else {
                GameMatrixAnimationTrack *created = GameMatrixAnimation_Start(
                    *(GameMatrixAnimation **)(gBattleContext + BATTLE_MAP_MATRIX_ANIMATION_OFFSET),
                    (const s16 *)buffers[7], func_ov002_020aaac8, 4096);
                *MapAnimationTrack(0) = created;
            }
            (*MapAnimationTrack(0))->flags.raw |= 4;
            (*MapAnimationTrack(0))->flags.bits.loop = 1;
        }
        BattleMap_ReadEntry(9, buffers, sizes);
        return;
    case 9:
        *(u32 *)(gBattleContext + BATTLE_MAP_SUB_PALETTE_CURSOR_OFFSET) = 0;
        Copy(buffers[9], gBattleContext + BATTLE_MAP_SUB_PALETTE_OFFSET, 480);
        ReadMapAnimationPair(14, buffers, sizes);
        return;
    case 14:
        if (sizes[14])
            *(const s16 **)(gBattleContext + BATTLE_MAP_PALETTE_COMMANDS_OFFSET + 4) =
                GameAnimationTable_GetComponent((const s16 *)buffers[14], -1, -1);
        if (sizes[15])
            *(const s16 **)(gBattleContext + BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET) =
                (const s16 *)buffers[15];
        BattleMap_ReadCompressedEntry(8, buffers, sizes);
        return;
    case 8:
        BattleMap_ReadEntry(10, buffers, sizes);
        return;
    case 10:
        if (sizes[10])
            CopyTilemapRows(buffers, 10, func_020368bc);
        BattleMap_ReadEntry(11, buffers, sizes);
        return;
    case 11:
        if (sizes[11])
            CopyTilemapRows(buffers, 11, func_020367b0);
        BattleMap_ReadEntry(12, buffers, sizes);
        return;
    case 12:
        if (sizes[12])
            CopyTilemapRows(buffers, 12, func_020366a4);
        if (*(const s16 **)(gBattleContext + BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET))
            StartMapAnimation(1, *(const s16 **)(gBattleContext + BATTLE_MAP_SUB_ANIMATION_COMMANDS_OFFSET));
        task->callback = 0;
        resource->flags.raw &= ~0x40000000;
        return;
    }
}
