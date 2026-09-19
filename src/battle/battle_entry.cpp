#include <game/battle_transition.h>
extern "C" {
#include <game/battle_entry.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
#include <game/save_data.h>
#include <game/graphics_resource.h>
#include <game/random.h>
#include <nitro/gx_init.h>
extern u8 *gBattleSystem;
extern MtxFx44 data_ov002_020bea14;
extern MtxFx44 data_ov002_020be9d4;
extern u8 data_ov002_020be9b4[];
u32 GX_ResetBankForBG(void);
u32 GX_ResetBankForOBJ(void);
u32 GX_ResetBankForTex(void);
u32 GX_ResetBankForTexPltt(void);
u32 GX_ResetBankForSubBG(void);
u32 GX_ResetBankForSubOBJ(void);
u32 GX_ResetBankForBGExtPltt(void);
u32 GX_ResetBankForOBJExtPltt(void);
u32 GX_ResetBankForSubBGExtPltt(void);
u32 GX_ResetBankForSubOBJExtPltt(void);
u32 func_02035610(void);
void func_02035c00(int);
void func_020359c4(int);
void func_0203613c(int);
void func_02035b0c(int);
void func_02035aec(int);
void func_020358ac(int);
void func_02036988(vu32 *, int, int, int, int);
void func_02036ee0(int, int, int, int, int);
void func_02036f24(int, int, int, int);
void func_02036cc0(const MtxFx44 *);
void *BattleTransfer_EnqueueAfterMapping(void (*)(void), void *, int, int);
}
#define REG32(address) (*(vu32 *)(address))
#define REG16(address) (*(vu16 *)(address))
#define FRAME ((BattleFrameContextView *)gBattleContext)
#define ENTRY ((BattleEntrySaveView *)(gSaveData + 0x558))
#define DISPLAY_FLAGS (&ENTRY->flags)

/* Function order follows MWCC reverse emission. */
extern "C" void BattleEntry_ConfigureVideoBanks(void)
{
    GX_ResetBankForBG();
    GX_ResetBankForOBJ();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();
    GX_ResetBankForSubBG();
    GX_ResetBankForSubOBJ();
    func_02035610();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForSubOBJExtPltt();
    func_02035c00(7);
    func_020359c4(8);
    func_0203613c(16);
    func_02035b0c(96);
    func_02035aec(128);
    func_020358ac(256);
    FRAME->texture_banks[0] = 0x06800000;
    FRAME->texture_banks[1] = 0x06820000;
    FRAME->palette_banks[0] = 0x06890000;
    FRAME->palette_banks[1] = 0x06894000;
    *(const u32 **)(gBattleSystem + 3576) = FRAME->texture_banks;
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    REG32(0x04000000) = REG32(0x04000000) & 0xF8FFFFFF;
    REG32(0x04000000) = REG32(0x04000000) & 0xC7FFFFFF;
    REG16(0x0400000A) = (REG16(0x0400000A) & 0x43) | 0x5A00;
    REG16(0x0400000C) = (REG16(0x0400000C) & 0x43) | 0x5C00;
    REG16(0x0400000E) = (REG16(0x0400000E) & 0x43) | 0x5E00;
    REG32(0x04000000) = (REG32(0x04000000) & 0xFFCFFFEF) | 0x10;
    REG16(0x04000008) = REG16(0x04000008) & ~3;
    REG16(0x0400000A) = (REG16(0x0400000A) & ~3) | 1;
    REG16(0x0400000C) = (REG16(0x0400000C) & ~3) | 2;
    REG16(0x0400000E) = (REG16(0x0400000E) & ~3) | 3;
    REG32(0x04000010) = 0;
    func_02036988((vu32 *)0x04000050, 1, 63, 16, 16);
    REG16(0x04001008) = (REG16(0x04001008) & 0x43) | 0x18;
    REG16(0x0400100A) = (REG16(0x0400100A) & 0x43) | 0x4204;
    REG16(0x0400100C) = (REG16(0x0400100C) & 0x43) | 0x4404;
    REG16(0x0400100E) = (REG16(0x0400100E) & 0x43) | 0x4604;
    REG32(0x04001000) = (REG32(0x04001000) & 0xFFCFFFEF) | 0x200010;
    REG16(0x04001008) = REG16(0x04001008) & ~3;
    REG16(0x0400100A) = (REG16(0x0400100A) & ~3) | 1;
    REG16(0x0400100C) = (REG16(0x0400100C) & ~3) | 2;
    REG16(0x0400100E) = (REG16(0x0400100E) & ~3) | 3;
    func_02036988((vu32 *)0x04001050, 1, 63, 16, 16);
    FRAME->render_values_30[1] = 16;
    FRAME->render_values_30[0] = 16;
    FRAME->render_values_30[3] = 1;
    FRAME->render_values_30[2] = 63;
    REG32(0x04001010) = 0;
    func_02036ee0(0, 0, 32767, 63, 0);
    REG16(0x04000060) = REG16(0x04000060) & ~0x3002;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 0x10;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    REG16(0x04000060) = REG16(0x04000060) & 0xCFFB;
    REG16(0x04000060) = REG16(0x04000060) & 0xCFDF;
    func_02036f24(0, 0, 0, 0);
    REG32(0x04000580) = 0xBFFF0000;
    REG32(0x04000540) = 1;
    GameGraphics_SetOrthographicProjection(0, -8192);
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1F00) | 0x100;
    REG32(0x04001000) = (REG32(0x04001000) & ~0x1F00) | 0x1000;
    GXi_SetMasterBrightness((vu16 *)0x0400006C, -16);
    GXi_SetMasterBrightness((vu16 *)0x0400106C, 0);
    if (DISPLAY_FLAGS->sub_screen)
        REG16(0x04000304) &= ~0x8000;
    else
        REG16(0x04000304) |= 0x8000;
}

extern "C" void BattleEntry_InitializeGraphics(void)
{
    /* Prime the 3D pipeline with the original textured quad command sequence. */
    REG32(0x040004A4) = 0x001F00C0;
    REG32(0x040004A8) = 0x5EDC8000;
    data_ov002_020bea14._31 = 24576;
    REG32(0x04000440) = 2;
    func_02036cc0(&data_ov002_020bea14);
    REG32(0x04000440) = 3;
    REG32(0x04000444) = 0;
    REG32(0x04000480) = 0x7FFF;
    func_02036cc0(&data_ov002_020be9d4);
    REG32(0x04000500) = 1;
    REG32(0x04000488) = 0xFD00FC00;
    REG32(0x0400048C) = 0xD000C000;
    REG32(0x0400048C) = 0;
    REG32(0x04000488) = 0xFD000400;
    REG32(0x0400048C) = 0xD0004000;
    REG32(0x0400048C) = 0;
    REG32(0x04000488) = 0x03000400;
    REG32(0x0400048C) = 0x2F804000;
    REG32(0x0400048C) = 0;
    REG32(0x04000488) = 0x0300FC00;
    REG32(0x0400048C) = 0x2F80C000;
    REG32(0x0400048C) = 0;
    REG32(0x04000504) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000448) = 1;
    REG32(0x04000454) = 0;
    /* One fixed sixteen-color palette and the live 256-color battle palette. */
    GameTexturePalette_Allocate((GameTexturePalette *)(gBattleContext + 26844), 2, 3, 16, 0, 1, 0,
                                data_ov002_020be9b4, 0, 0);
    GameTexturePalette_Allocate((GameTexturePalette *)(gBattleContext + 52100), 2, 4, 256, 0, 0, 0,
                                FRAME->main_palette, 0x3021, 32);
    BattleAI_Initialize();
    BattleTransition_BeginEntry();
    BattleTransfer_EnqueueAfterMapping(BattleEntry_ShowBattleDisplay, 0, 0, 0);
    FRAME->main_brightness_level = 32;
    FRAME->sub_brightness_level = 0;
    FRAME->vblank = BattleMain_VBlank;
    FRAME->update = BattleEntry_WaitArchives;
}

extern "C" void BattleEntry_ShowBattleDisplay(void)
{
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1F00) | 0x100;
    REG32(0x04001000) = REG32(0x04001000) & ~0x1F00;
    GXi_SetMasterBrightness((vu16 *)0x0400006C, 0);
    GXi_SetMasterBrightness((vu16 *)0x0400106C, -16);
    if (DISPLAY_FLAGS->sub_screen)
        REG16(0x04000304) |= 0x8000;
    else
        REG16(0x04000304) &= ~0x8000;
}

extern "C" void BattleEntry_SeedRandomFromClock(u32 result, void *argument)
{
    Random_SetSeed(FRAME->clock.second + (60 * FRAME->clock.minute + 3600 * FRAME->clock.hour));
}

#include <game/battle_common_resources.h>
extern "C" {
#include <game/battle_archive.h>
#include <game/battle_task_queue.h>
#include <game/battle_interface_assets.h>
#include <game/heap.h>
extern s16 data_ov002_020be9a8[];
void BattleItemList_RebuildActionItems(void);
void BattleItemList_RebuildUsableItems(void);
extern int data_ov002_020c071c;
extern u8 data_ov002_020beaac[], data_ov002_020beabc[], data_ov002_020beacc[], data_ov002_020beadc[],
    data_ov002_020beaec[], data_ov002_020beafc[], data_ov002_020beb0c[], data_ov002_020beb14[];

void *BattleTransfer_EnqueueBeforeMapping(int (*)(void *), void *, int, int);
void func_02009078(int, void *, void *);
void func_02037190(void);
}
enum BattleEntryStorageOffset {
    BATTLE_ENTRY_PALETTE_BUFFER_OFFSET = 0x57608,
    BATTLE_ENTRY_SUB_SPRITE_BUFFER_OFFSET = 0x5F608,
    BATTLE_ENTRY_SUB_SPRITE_AFFINES_OFFSET = 0x61608,
    BATTLE_ENTRY_SPRITE_ANIMATION_STORAGE_OFFSET = 0x12D68,
    BATTLE_ENTRY_MATRIX_ANIMATION_STORAGE_OFFSET = 0x10168
};
#define FRAME ((BattleFrameContextView *)gBattleContext)
#define HEAPS ((BattleEntryHeaps *)(gBattleContext + 0xE148))

#define CONTEXT ((BattleContext *)gBattleContext)
#define UI_OBJECT(storage) (*(BattleSceneObject **)(storage))

static inline SavePartyMember *SavedParty(BattlePartyActorStorage *storage)
{
    return (SavePartyMember *)storage->party.actor.resource_slot;
}

extern "C" void BattleEntry_InitializeObjects(BattleQueuedTask *task)
{
    for (int i = 0; i < 72; ++i)
        CONTEXT->runtime.object_data_load_states[i].object_data_id = i;
    /* Bind UI storage to its shared scene objects; these interfaces own their layouts. */
    UI_OBJECT(FRAME->target_marker) = &FRAME->scene[0];
    FRAME->ui_scene_aliases[0] = UI_OBJECT(FRAME->target_marker);
    FRAME->ui_scene_aliases[1] = FRAME->ui_scene_aliases[0];
    UI_OBJECT(FRAME->target_marker + 8) = &FRAME->scene[1];
    UI_OBJECT(FRAME->target_cursor) = &FRAME->scene[2];
    UI_OBJECT(FRAME->command_wheel) = UI_OBJECT(FRAME->target_cursor);
    UI_OBJECT(FRAME->hp_members[2]) = &FRAME->scene[3];
    UI_OBJECT(FRAME->hp_members[0]) = UI_OBJECT(FRAME->hp_members[2]);
    UI_OBJECT(FRAME->hp_members[3]) = &FRAME->scene[4];
    UI_OBJECT(FRAME->hp_members[1]) = UI_OBJECT(FRAME->hp_members[3]);
    UI_OBJECT(FRAME->hp_panel) = &FRAME->scene[5];
    UI_OBJECT(FRAME->command_menu) = &FRAME->scene[6];
    FRAME->ui_scene_aliases[3] = UI_OBJECT(FRAME->command_menu);
    UI_OBJECT(FRAME->target_label) = &FRAME->scene[7];
    FRAME->ui_scene_aliases[4] = UI_OBJECT(FRAME->target_label);
    UI_OBJECT(FRAME->party_indicators[0]) = &FRAME->scene[10];
    UI_OBJECT(FRAME->party_indicators[1]) = &FRAME->scene[11];
    FRAME->ui_scene_aliases[2] = UI_OBJECT(FRAME->party_indicators[1]);
    UI_OBJECT(FRAME->target_label + 84) = &FRAME->scene[12];
    UI_OBJECT(FRAME->results) = UI_OBJECT(FRAME->target_label + 84);
    *(u16 *)(FRAME->party_indicators[0] + 8) = 56;
    *(u16 *)(FRAME->party_indicators[1] + 8) = 57;
    *(s16 *)(FRAME->target_marker + 12) = -1;
    /* Slots 56..59 are party members, 60..67 enemies, and 68..69 cameras. */
    for (int i = 0; i < 70; ++i) {
        FRAME->scene[i].actor_id = i;
        FRAME->scene[i].linked_actor_id = FRAME->scene[i].actor_id;
        FRAME->scene[i].render_state = 0x7fff;
        if (i < 56)
            FRAME->scene_table[i] = &FRAME->scene[i];
        if (i >= 68)
            FRAME->camera[i - 68] = &FRAME->scene[i];
    }
    for (int i = 0; i < 8; ++i)
        *(u16 *)CONTEXT->enemy_data_requests[i].unknown_08 = 72 + i;
    for (int i = 0; i < 4; ++i) {
        BattlePartyActorStorage *storage = &FRAME->party_storage[i];
        FRAME->party[i] = &storage->party;
        /* Keep the party subarray as the base of the native scene traversal. */
        storage->party.actor.scene_object = &FRAME->scene[56] + i;
        storage->party.actor.resource_slot = &((SavePartyMember *)(gSaveData + 1016))[i];
        storage->party.actor.current_hp = SavedParty(storage)->current_hp;
        storage->party.actor.max_hp = SavedParty(storage)->max_hp;
        storage->party.actor.speed = SavedParty(storage)->speed;
        storage->party.actor.base_speed = storage->party.actor.speed;
        storage->party.actor.power = SavedParty(storage)->power;
        storage->party.actor.base_power = storage->party.actor.power;
        storage->party.actor.defense = SavedParty(storage)->defense;
        storage->party.actor.base_defense = storage->party.actor.defense;
        storage->party.actor.unk_00e = SavedParty(storage)->stache;
        storage->party.actor.unk_016 = storage->party.actor.unk_00e;
        storage->party.actor.flag_bits.level = SavedParty(storage)->experience.fields.level;
        storage->party.formation_index = i;
        storage->party.linked_object_id = ((i & 1) ^ 1) + 56;
        storage->party.formation_value = data_ov002_020be9a8[storage->party.formation_index];
        storage->initial_parameters[0] = 1;
        storage->initial_parameters[1] = 4096;
        storage->initial_parameters[2] = 8192;
        storage->initial_parameters[3] = 56;
        storage->initial_parameters[4] = 60;
    }
    if (((BattleEntrySaveView *)(gSaveData + 0x558))->formation == 2) {
        FRAME->party[0]->formation_index = 4;
        FRAME->party[1]->formation_index = 5;
    }
    for (int i = 0; i < 8; ++i) {
        BattleEnemyActor *actor = &FRAME->enemy_storage[i];
        CONTEXT->enemy_actors[i] = &actor->actor;
        actor->actor.scene_object = &FRAME->scene[60] + i;
    }
    FRAME->initial_ui_parameters[0] = 1;
    FRAME->initial_ui_parameters[1] = 53;
    FRAME->initial_ui_parameters[2] = 8;
    FRAME->initial_ui_parameters[3] = 18;
    BattleItemList_RebuildActionItems();
    BattleItemList_RebuildUsableItems();
    task->callback = BattleEntry_InitializeHeaps;
}

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
    BattleTransfer_EnqueueBeforeMapping(BattleMain_UploadTextures, 0, 0, 0);
    task->callback = 0;
}

extern "C" void BattleEntry_WaitArchives(void)
{
    BattleTaskList_Update(&FRAME->task_list_8b44);
    if (*(u16 *)(gBattleSystem + 1512) == *(u16 *)(gBattleSystem + 1514)) {
        BattleCommonResources_Initialize(gBattleContext);
        FRAME->update = BattleEntry_LoadResources;
    }
}

extern "C" {
void func_ov002_02068878(BattleObjectDataLoadState *, int, int);

void func_ov002_0206f1f0(BattleModel *, BattleObjectDataLoadState *, int);
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
extern u8 data_ov002_020bea94[], data_ov002_020bea9c[];
}

#define ENTRY ((BattleEntrySaveView *)(gSaveData + 0x558))
#define FRAME ((BattleFrameContextView *)gBattleContext)

extern "C" void BattleEntry_LoadResources(void)
{
    BattleTaskList_Update(&FRAME->task_list_8b44);
    BattleObjectData_AllocateLoadBuffer(0, 98304);
    BattleObjectData_AllocateLoadBuffer(1, 98304);
    BattleObjectData_AllocateLoadBuffer(5, 40960);
    BattleObjectData_AllocateLoadBuffer(6, 40960);
    BattleObjectData_AllocateLoadBuffer(2, 4096);
    BattleObjectData_AllocateLoadBuffer(3, 0x4000);
    BattleObjectData_AllocateLoadBuffer(4, 7168);
    BattleObjectData_AllocateLoadBuffer(13, 4096);
    BattleObjectData_AllocateLoadBuffer(7, 4096);
    BattleObjectData_AllocateLoadBuffer(8, 2048);
    BattleObjectData_AllocateLoadBuffer(9, 18432);
    BattleObjectData_AllocateLoadBuffer(19, 20480);
    BattleObjectData_AllocateLoadBuffer(18, 4096);
    BattleObjectData_AllocateLoadBuffer(17, 36864);
    BattleObjectData_AllocateLoadBuffer(16, 0x4000);
    BattleObjectData_AllocateLoadBuffer(15, 0x4000);
    BattleObjectData_ConfigureLoad(0, 0, 1);
    BattleObjectData_ConfigureLoad(1, 128, 1);
    BattleObjectData_ConfigureLoad(5, 0, 1);
    BattleObjectData_ConfigureLoad(6, 128, 1);
    BattleObjectData_ConfigureLoad(2, 992, 0);
    BattleObjectData_ConfigureLoad(3, 512, 0);
    BattleObjectData_ConfigureLoad(4, 660, 0);
    BattleObjectData_ConfigureLoad(13, 2432, 0);
    BattleObjectData_ConfigureLoad(7, 2512, 0);
    BattleObjectData_ConfigureLoad(8, 686, 0);
    BattleObjectData_ConfigureLoad(9, 704, 0);
    BattleObjectData_ConfigureLoad(19, 2048, 0);
    BattleObjectData_ConfigureLoad(18, 2268, 0);
    BattleObjectData_ConfigureLoad(16, 2048, 0);
    BattleObjectData_ConfigureLoad(15, 2268, 0);
    func_ov002_02068878(BattleObjectData_GetLoadState(17), 0, 0);
    BattleObjectData_AllocateLoadBuffer(53, 0x10000);
    if (ENTRY->flags.luigi) {
        BattleObjectData_ConfigureLoad(53, 128, 1);
        switch (ENTRY->flags.approach) {
        /* Keep the explicit empty branch represented in the native jump table. */
        case -2:
            break;
        case -1:
        case 1:
            switch (ENTRY->formation) {
            case 0:
                BattleObjectData_EnsureLoaded(53, 0xC0000008);
                break;
            case 1:
                BattleObjectData_EnsureLoaded(53, 0xC000000A);
                break;
            case 2:
                BattleObjectData_EnsureLoaded(53, 0xC000000E);
                break;
            }
            break;
        case 2:
            switch (ENTRY->formation) {
            case 1:
                BattleObjectData_EnsureLoaded(53, 0xC0000010);
                break;
            case 2:
                BattleObjectData_EnsureLoaded(53, 0xC0000026);
                break;
            }
            break;
        }
    } else {
        BattleObjectData_ConfigureLoad(53, 0, 1);
        switch (ENTRY->flags.approach) {
        /* Keep the explicit empty branch represented in the native jump table. */
        case -2:
            break;
        case -1:
        case 1:
            switch (ENTRY->formation) {
            case 0:
                BattleObjectData_EnsureLoaded(53, 0xC0000007);
                break;
            case 1:
                BattleObjectData_EnsureLoaded(53, 0xC0000009);
                break;
            case 2:
                BattleObjectData_EnsureLoaded(53, 0xC000000C);
                break;
            }
            break;
        case 2:
            switch (ENTRY->formation) {
            case 1:
                BattleObjectData_EnsureLoaded(53, 0xC000000F);
                break;
            case 2:
                BattleObjectData_EnsureLoaded(53, 0xC0000025);
                break;
            }
            break;
        }
    }
    switch (ENTRY->formation) {
    case 0:
        BattleObjectData_EnsureLoaded(0, 0xC0000000);
        BattleObjectData_EnsureLoaded(1, 0xC0000001);
        FRAME->party[0]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        FRAME->party[1]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        break;
    case 1:
        BattleObjectData_EnsureLoaded(0, 0xC0000002);
        BattleObjectData_EnsureLoaded(1, 0xC0000003);
        FRAME->party[0]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        FRAME->party[1]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        break;
    case 2:
        BattleObjectData_EnsureLoaded(0, 0xC000000B);
        BattleObjectData_EnsureLoaded(1, 0xC000000D);
        FRAME->party[0]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        FRAME->party[1]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        FRAME->party[2]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        FRAME->party[3]->actor.flags |= BATTLE_ACTOR_FLAG_RESOURCE_BOUND;
        break;
    }
    func_ov002_02091fd8(2, 0xC1000000);
    func_ov002_02091fd8(3, 0xC200003C);
    BattleObjectData_EnsureLoaded(4, 0xC2000001);
    BattleObjectData_EnsureLoaded(8, 0xC200001A);
    BattleObjectData_EnsureLoaded(9, 0xC0000052);
    BattleObjectData_EnsureLoaded(19, 0xC10000B6);
    BattleObjectData_EnsureLoaded(18, 0xC1000005);
    BattleObjectData_EnsureLoaded(17, 0xC100001F);
    func_ov002_02091fd8(16, 0xC10000B6);
    func_ov002_02091fd8(15, 0xC1000005);
    BattleObjectData_EnsureLoaded(7, 0xC2000013);
    BattleObjectData_EnsureLoaded(4, 0xC2000001);
    FRAME->update = BattleEntry_InitializeScene;
}

#define ENTRY_MODEL FRAME->party_animation_model
#define ENTRY_CONTROLLER FRAME->party_animation_data

static inline void PlaceObject(int id, int x, int y, int z)
{
    BattleSceneObject *object = BattleSceneObject_GetById(id);
    BattleSceneObject_AdjustPosition(object, x - object->x, y - object->y, z - object->z);
}
static inline void RememberPosition(int id)
{
    BattleActor *actor = BattleActor_GetPartySlot(id);
    actor->unk_018 = actor->scene_object->x;
    actor->unk_01a = actor->scene_object->y;
    actor->unk_01c = actor->scene_object->z;
}
static inline BattleSceneObject *UiObject(void *storage)
{
    return *(BattleSceneObject **)storage;
}

extern "C" void BattleEntry_InitializeScene(void)
{
    BattleTaskList_Update(&FRAME->task_list_8b44);
    if (BattleObjectData_IsLoadPending(4))
        return;
    if (!BattleCommonResources_UpdateLoad())
        return;
    if (ENTRY->formation == 1) {
        BattleSceneObject_SwapSlots(56, 58);
        BattleSceneObject_SwapSlots(57, 59);
    }
    BattleEntity_BindResource(56, 0);
    BattleEntity_BindResource(57, 1);
    BattleEntity_BindResource(58, 0);
    BattleEntity_BindResource(59, 1);
    BattleEntity_BindResource(0, 4);
    BattleEntity_BindResource(1, 4);
    BattleEntity_BindResource(10, 4);
    BattleEntity_BindResource(11, 4);
    /* The original repeats this binding. */
    BattleEntity_BindResource(11, 4);
    BattleEntity_BindResource(5, 7);
    /* Repeated placements are intentional: each native call propagates motion deltas. */
    PlaceObject(56, 56, 112, 0);
    PlaceObject(57, 46, 152, 0);
    PlaceObject(56, 56, 112, 0);
    PlaceObject(57, 46, 152, 0);
    PlaceObject(58, 56, 112, 0);
    PlaceObject(59, 46, 152, 0);
    PlaceObject(58, 56, 112, 0);
    PlaceObject(59, 46, 152, 0);
    PlaceObject(3, 85, 510, 330);
    PlaceObject(4, 32, 510, 330);
    PlaceObject(5, 176, 510, 446);
    PlaceObject(6, 192, 176, 0);
    PlaceObject(7, 192, 16, 0);
    RememberPosition(56);
    RememberPosition(57);
    RememberPosition(58);
    RememberPosition(59);
    UiObject(FRAME->command_wheel)->flags.bits.use_raw_position = 0;
    UiObject(FRAME->target_cursor)->flags.bits.use_raw_position = 0;
    UiObject(FRAME->hp_members[0])->flags.bits.use_raw_position = 1;
    UiObject(FRAME->hp_members[1])->flags.bits.use_raw_position = 1;
    UiObject(FRAME->hp_members[2])->flags.bits.use_raw_position = 1;
    UiObject(FRAME->hp_members[3])->flags.bits.use_raw_position = 1;
    UiObject(FRAME->hp_panel)->flags.bits.use_raw_position = 1;
    UiObject(FRAME->command_menu)->flags.bits.use_raw_position = 1;
    UiObject(FRAME->target_label)->flags.bits.use_raw_position = 1;
    UiObject(FRAME->target_label + 84)->flags.bits.use_raw_position = 1;
    UiObject(FRAME->target_marker)->flags.bits.use_raw_position = 0;
    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(56), 0, -1);
    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(57), 0, -1);
    BattleModel *model = (BattleModel *)GameHeap_New(
        sizeof(BattleModel), ((BattleEntryHeaps *)(gBattleContext + 0xE148))->texture_animation,
        data_ov002_020bea94, 0);
    if (model)
        model = BattleRenderModel_Init(model);
    ENTRY_MODEL = model;
    BattleObjectDataLoadState *resource = BattleObjectData_GetLoadState(0);
    func_ov002_0206f1f0(ENTRY_MODEL, resource, 0);
    ENTRY_MODEL->stop();
    ENTRY_MODEL->flags &= ~0x100u;
    u32 size = ((u8 *)resource->component_0c - (u8 *)resource->component_08 + 3) & ~3;
    ENTRY_CONTROLLER = GameHeap_Allocate(((BattleEntryHeaps *)(gBattleContext + 0xE148))->main_data, size,
                                         data_ov002_020bea9c, 1);
    if (resource->component_08 < ENTRY_CONTROLLER)
        func_0202cd2c(resource->component_08, ENTRY_CONTROLLER, size);
    else
        func_0202cc58(resource->component_08, ENTRY_CONTROLLER, size);
    ENTRY_MODEL->animation_controller = ENTRY_CONTROLLER;
    u16 initiator = ENTRY->flags.luigi ? 57 : 56;
    BattleModel *active = BattleSceneObject_GetActiveModelById(initiator);
    switch (ENTRY->flags.approach) {
    case -1:
    case 1:
        BattleEntity_BindResource(initiator, 53);
        PlaceObject(initiator, 192, 128, 32);
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(initiator), 1, -1);
        active->flags |= 0x200;
        break;
    case 2:
        BattleEntity_BindResource(initiator, 53);
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(initiator), 6, -1);
        active->flags |= 0x200;
        break;
    case -2:
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(initiator), 24, -1);
        active->flags |= 0x200;
        break;
    }
    BattleAI_StartPartyVmSlot3();
    FRAME->turn_state = 1;
    FRAME->update = BattleMain_Update;
}
