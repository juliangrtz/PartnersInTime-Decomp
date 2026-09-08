#include <game/battle_archive.h>
#include <game/archive_io.h>
#include <game/battle_scene.h>

extern void *gBattleSystem;
extern const void *data_ov002_020bebd0;
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
u8 *func_020365fc(void);
void func_ov002_0208ad54(BattleQueuedTask *);
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
    task->callback = func_ov002_0208ad54;
}
