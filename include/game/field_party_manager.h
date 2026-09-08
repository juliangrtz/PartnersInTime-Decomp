#ifndef PIT_GAME_FIELD_PARTY_MANAGER_H
#define PIT_GAME_FIELD_PARTY_MANAGER_H
#include <game/field_party_snapshot.h>
#include <game/field_area.h>
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
    u8 unknown_4168[20];
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
void FieldPartyManager_SaveSnapshot(FieldPartyManager *manager, FieldPartyManagerSnapshot *snapshot);
FieldPartyManager *FieldPartyManager_DestroyStorage(FieldPartyManager *manager);
FieldPartyManager *FieldPartyManager_InitializeStorage(FieldPartyManager *manager);
#ifdef __cplusplus
}
#endif
#endif
