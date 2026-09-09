#ifndef PIT_GAME_FIELD_PARTY_MANAGER_H
#define PIT_GAME_FIELD_PARTY_MANAGER_H
#include <game/field_party_snapshot.h>
#include <game/field_area.h>
typedef struct FieldPartyPendingTransition {
    u16 active : 1, side : 1, direction : 3, entry_slide : 1;
    s16 bgm : 8;
    u16 option_9 : 1, option_10 : 1;
    s16 room, script, x, y, z;
} FieldPartyPendingTransition;
typedef char FieldPartyPendingTransition_SizeCheck[sizeof(FieldPartyPendingTransition) == 12 ? 1 : -1];

typedef struct FieldPartyManager {
    FieldAreaContext **areas;
    union {
        u32 flags;
        struct {
            u32 active_party : 1, unknown_01_06 : 6, unknown_07 : 1;
            s32 unknown_08_11 : 4;
            u32 unknown_12 : 1, unknown_13 : 1, unknown_14 : 1, unknown_15 : 1;
            u32 unknown_16 : 1, unknown_17_31 : 15;
        } bits;
    };
    u8 unknown_08[16];
    void *owner;
    u32 unknown_1c;
    FieldPartyController parties[2];
    FieldPartyPendingTransition pending_transition;
    struct { u16 active : 1, side : 1, unknown_02_15 : 14; } pending_bounds;
    u16 unknown_4176;
    const FieldPairedBoundsRuntime *pending_bounds_record;
} FieldPartyManager;
typedef char FieldPartyManager_SizeCheck[sizeof(FieldPartyManager) == 16764 ? 1 : -1];
typedef struct FieldPartyManagerSnapshot {
    struct {
        u32 active_party : 1, unknown_07 : 1;
        s32 unknown_08_11 : 4;
        u32 unknown_12 : 1, unknown_13 : 1, unknown_14 : 1, unknown_16 : 1, unknown_10_31 : 22;
    } flags;
    FieldPartyControllerSnapshot parties[2];
} FieldPartyManagerSnapshot;
typedef char FieldPartyManagerSnapshot_SizeCheck[sizeof(FieldPartyManagerSnapshot) == 772 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
/* Both groups must be in ordinary field mode before beginning reunion. */
void FieldPartyManager_BeginReunion(FieldPartyManager *manager, int side, int instant);
int FieldPartyManager_IsReuniting(FieldPartyManager *manager);
void FieldPartyManager_CompleteReunion(FieldPartyManager *manager);
void FieldPartyManager_SaveSnapshot(FieldPartyManager *manager, FieldPartyManagerSnapshot *snapshot);
FieldPartyManager *FieldPartyManager_DestroyStorage(FieldPartyManager *manager);
FieldPartyManager *FieldPartyManager_InitializeStorage(FieldPartyManager *manager);
void FieldPartyManager_CompleteAirborneTransfers(FieldPartyManager *manager);
void FieldPartyManager_BeginPiggybackMount(FieldPartyManager *manager);
int FieldPartyManager_IsPiggybackMountActive(FieldPartyManager *manager);
void FieldPartyManager_CancelReunion(FieldPartyManager *manager);
void FieldPartyManager_PrepareBabyDrillMarker(FieldPartyManager *manager,int animation);
void FieldPartyManager_MoveToAnchor(FieldPartyManager *manager,int side,int index);
void FieldPartyManager_PrepareBrosBallLaunch(FieldPartyManager *manager);
void FieldPartyManager_LaunchBrosBall(FieldPartyManager *manager,int direction);
void FieldPartyManager_LaunchBabiesToElevation(FieldPartyManager *manager,fx32 height);
void FieldPartyManager_DropBabies(FieldPartyManager *manager);

void FieldPartyManager_QueuePairedBounds(FieldPartyManager *manager, int side,
    const FieldPairedBoundsRuntime *record);
void FieldPartyManager_CheckRegion71Bounds(FieldPartyManager *manager, int side);
void FieldPartyManager_ApplyPendingTransition(FieldPartyManager *manager);
void FieldPartyManager_TransitionThroughBounds(FieldPartyManager *manager, int side,
    const FieldPairedBoundsRuntime *record);

void FieldPartyManager_SetFieldMode(FieldPartyManager *manager,int side,u16 mode,int force,int preserve);
int FieldPartyManager_HasActiveActions(FieldPartyManager *manager,int side,int present_party_mask);
void FieldPartyManager_CancelActions(FieldPartyManager *manager,int side,u16 mask);

#ifdef __cplusplus
}
#endif
#endif
