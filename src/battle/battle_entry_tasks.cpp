extern "C" {
#include <game/battle_entry.h>
#include <game/battle_archive.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
#include <game/battle_task_queue.h>
#include <game/battle_interface_assets.h>
#include <game/heap.h>
#include <game/save_data.h>
extern s16 data_ov002_020be9a8[];
void BattleItemList_RebuildActionItems(void);
void BattleItemList_RebuildUsableItems(void);
extern u8 *gBattleSystem;
extern int data_ov002_020c071c;
extern u8 data_ov002_020beaac[], data_ov002_020beabc[], data_ov002_020beacc[], data_ov002_020beadc[],
    data_ov002_020beaec[], data_ov002_020beafc[], data_ov002_020beb0c[], data_ov002_020beb14[];
void func_ov002_0206a44c(void *);
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
    func_ov002_020725a4(BattleMain_UploadTextures, 0, 0, 0);
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
