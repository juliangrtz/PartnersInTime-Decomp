#include <game/field_system.h>
#include <game/field_resources.h>
#include <game/sprite_window.h>
#include <game/heap.h>
#include <game/save_data.h>
typedef struct SavePartyDataView {
    u8 unknown_000[1016];
    SavePartyMember members[4];
} SavePartyDataView;
typedef char SavePartyDataView_SizeCheck[sizeof(SavePartyDataView) == 1160 ? 1 : -1];
extern void func_ov000_02071c08(FieldAreaContext *, int);
void FieldSystem_ReleaseSharedResources(FieldSystem *system)
{
    int i;
    for (i = 0; i < 85; ++i) {
        GameHeap_DeleteArray(system->shared_primary[i].graphics);
        GameHeap_DeleteArray(system->shared_primary[i].animation);
        if (system->shared_primary[i].bounds)
            GameHeap_DeleteArray(system->shared_primary[i].bounds);
    }
    {
        int j;
        for (j = 0; j < 2; ++j)
            GameHeap_DeleteArray(system->shared_secondary[j].data);
    }
    GameHeap_DeleteArray(system->shared_secondary);
    GameHeap_DeleteArray(system->shared_primary);
}
void FieldSystem_CreateSpriteWindows(FieldSystem *system)
{
    GameSpriteWindowManager *windows = GameHeap_New(sizeof(GameSpriteWindowManager), 0, 0, 0);
    if (windows)
        windows = GameSpriteWindow_Construct(windows, 8, 0, 0, 0, 0, 0, 0, 0, 1, 0);
    system->windows = windows;
    GameWindow_AllocatePool(&system->windows->base, 0, 8, 0);
    GameSpriteWindow_AllocateSlots(system->windows, 0, 2);
}
void FieldSystem_RefillMemberHp(FieldSystem *system, int member)
{
    SavePartyDataView *save = (SavePartyDataView *)gSaveData;
    save->members[member].current_hp = save->members[member].max_hp;
    if (system->areas[1] && system->areas[1]->unknown_23f0 == 71)
        func_ov000_02071c08(system->areas[1], 0);
}
void FieldSystem_AdjustMemberHp(FieldSystem *system, int member, int amount)
{
    SavePartyDataView *save = (SavePartyDataView *)gSaveData;
    int current = save->members[member].current_hp;
    int maximum = save->members[member].max_hp;
    int hp = current + amount;
    if (hp > maximum)
        hp = maximum;
    else if (hp < 0)
        hp = 0;
    save->members[member].current_hp = hp;
    if (system->areas[1] && system->areas[1]->unknown_23f0 == 71)
        func_ov000_02071c08(system->areas[1], 0);
}
