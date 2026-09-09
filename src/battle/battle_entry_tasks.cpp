extern "C" {
#include <game/battle_entry.h>
#include <game/battle_archive.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
#include <game/battle_task_queue.h>
#include <game/battle_interface_assets.h>
#include <game/heap.h>
extern u8 *gBattleSystem;
extern int data_ov002_020c071c;
extern u8 data_ov002_020beaac[], data_ov002_020beabc[], data_ov002_020beacc[], data_ov002_020beadc[],
    data_ov002_020beaec[], data_ov002_020beafc[], data_ov002_020beb0c[], data_ov002_020beb14[];
void func_ov002_0206a44c(void *);
int func_ov002_02077268(void *);
void *func_ov002_020725a4(int (*)(void *), void *, int, int);
void func_02009078(int, void *, void *);
void func_02037190(void);
}
enum BattleEntryStorageOffset {
    BATTLE_ENTRY_PALETTE_BUFFER_OFFSET = 0x57608,
    BATTLE_ENTRY_SUB_SPRITE_BUFFER_OFFSET = 0x5F608,
    BATTLE_ENTRY_SUB_SPRITE_AFFINES_OFFSET = 0x61608,
    BATTLE_ENTRY_SPRITE_ANIMATION_STORAGE_OFFSET = 0x12D68,
    BATTLE_ENTRY_MATRIX_ANIMATION_STORAGE_OFFSET = 0x10168,
};

#define FRAME ((BattleFrameContextView *)gBattleContext)
#define HEAPS ((BattleEntryHeaps *)(gBattleContext + 0xE148))

extern "C" void BattleEntry_InitializeHeaps(BattleQueuedTask *task)
{
    HEAPS->main_data = GameHeap_Create(0, 720896, (u32)data_ov002_020beaac, 1);
    HEAPS->sub_data = GameHeap_Create(0, 163840, (u32)data_ov002_020beabc, 1);
    data_ov002_020c071c = HEAPS->sub_data;
    HEAPS->scene_data = GameHeap_Create(0, 239616, (u32)data_ov002_020beacc, 1);
    HEAPS->attack_data = GameHeap_Create(0, 229376, (u32)data_ov002_020beadc, 1);
    HEAPS->texture_animation = GameHeap_Create(0, 43776, (u32)data_ov002_020beaec, 1);
    HEAPS->parameter_animation = GameHeap_Create(0, 20480, (u32)data_ov002_020beafc, 1);
    BattleTaskPool_Init(&FRAME->task_list_8b44, 64, 16);
    BattleTaskPool_Init(&FRAME->task_list_924c, 8, 16);
    BattleTaskPool_Init(&FRAME->task_list_9334, 12, 12);
    BattleTaskPool_Init(&FRAME->task_list_945c, 192, 32);
    BattleTaskPool_Init(&FRAME->task_list_b564, 128, 24);
    BattleTaskPool_Init(&FRAME->task_list_c76c, 16, 8);
    GameTexturePalette_SetBuffer(gBattleContext + BATTLE_ENTRY_PALETTE_BUFFER_OFFSET, 0x8000);
    func_02009078(1, gBattleContext + BATTLE_ENTRY_SUB_SPRITE_BUFFER_OFFSET,
                  gBattleContext + BATTLE_ENTRY_SUB_SPRITE_AFFINES_OFFSET);
    func_02037190();
    GameSpriteAnimation *sprite =
        (GameSpriteAnimation *)GameHeap_New(sizeof(GameSpriteAnimation), 0, data_ov002_020beb0c, 0);
    if (sprite)
        sprite = GameSpriteAnimation_Initialize(
            sprite, gBattleContext + BATTLE_ENTRY_SPRITE_ANIMATION_STORAGE_OFFSET, 128, 4096);
    FRAME->sprite_animation = sprite;
    GameMatrixAnimation *matrix =
        (GameMatrixAnimation *)GameHeap_New(sizeof(GameMatrixAnimation), 0, data_ov002_020beb14, 0);
    if (matrix)
        matrix = GameMatrixAnimation_Initialize(
            matrix, gBattleContext + BATTLE_ENTRY_MATRIX_ANIMATION_STORAGE_OFFSET, 128, 64);
    FRAME->matrix_animation = matrix;
    task->callback = BattleEntry_LoadArchives;
}

extern "C" void BattleEntry_LoadArchives(BattleQueuedTask *task)
{
    BattleTaskQueue_Enqueue(BattleTextureCatalog_OpenTask, (void *)0xC0000000);
    BattleTaskQueue_Enqueue(BattleTextureCatalog_OpenTask, (void *)0xC1000000);
    BattleTaskQueue_Enqueue(BattleTextureCatalog_OpenTask, (void *)0xC2000000);
    BattleInterface_RequestScreenLoad(0);
    BattleInterface_RequestScreenLoad(1);
    BattleInterface_RequestLoad();
    func_ov002_020725a4(func_ov002_02077268, 0, 0, 0);
    task->callback = 0;
}

extern "C" void BattleEntry_WaitArchives(void)
{
    BattleTaskList_Update(&FRAME->task_list_8b44);
    if (*(u16 *)(gBattleSystem + 1512) == *(u16 *)(gBattleSystem + 1514)) {
        func_ov002_0206a44c(gBattleContext);
        FRAME->update = BattleEntry_LoadResources;
    }
}
