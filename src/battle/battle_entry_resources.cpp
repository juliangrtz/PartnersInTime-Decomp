extern "C" {
#include <game/battle_entry.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
#include <game/save_data.h>
#include <game/heap.h>
void func_ov002_02068878(BattleObjectDataLoadState *, int, int);
int func_ov002_0206a240(void);
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
    if (!func_ov002_0206a240())
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
