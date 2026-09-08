#include <game/battle_archive.h>
#include <game/archive_io.h>
#include <game/battle_scene.h>
#include <game/matrix_animation.h>

extern void *gBattleSystem;
extern const void *data_ov002_020bebd0;
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
u8 *func_020365fc(void);
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
    BATTLE_MAP_RESOURCE_OFFSET = 52204,
    BATTLE_MAP_WORK_BUFFER_OFFSET = 226824,
    BATTLE_MAP_PALETTE_LOCATION_OFFSET = 52100,
    BATTLE_MAP_BUFFER_TABLE_OFFSET = 116,
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
    u8 **buffers = (u8 **)(resource->data + 64);
    u32 *sizes = (u32 *)(resource->data + 128);
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
