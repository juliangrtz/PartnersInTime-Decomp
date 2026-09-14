#ifndef PIT_GAME_FIELD_SYSTEM_H
#define PIT_GAME_FIELD_SYSTEM_H
#include <game/field_party_manager.h>
#include <game/task.h>
#include <game/field_resources.h>
#include <nitro/tp.h>
#include <nitro/fx_vec.h>
typedef struct FieldTimer FieldTimer;
typedef struct FieldModelAnimation FieldModelAnimation;
typedef struct FieldSpriteAnimation FieldSpriteAnimation;
typedef struct GameSpriteWindowManager GameSpriteWindowManager;
typedef struct FieldSystem {
    GameTask task;
    u8 unknown_18[16];
    union {
        u8 storage[912];
        struct {
            u8 unknown_28[24];
            GameTask *update_task;
            FieldArchive *archive;
            GameIrqTask *irq_task;
            FieldPrimaryResource *shared_primary;
            FieldSecondaryResource *shared_secondary, *temporary_secondary;
            u16 shared_palette[256];
            union {
                u8 unknown_258[16];
                struct {
                    struct { u16 phase : 4, unknown : 12; } scene_transition;
                    u16 unknown_25a, save_location;
                    u8 unknown_25e[10];
                };
            };
            struct {
                u32 enabled : 1, state : 3;
                s32 x : 9, y : 9;
                u32 previous_touch : 1, unknown_23_31 : 9;
            } touch;
            TpData touch_sample;
            union {
                u8 unknown_274[296];
                struct {
                    u8 unknown_274_prefix[252];
                    VecFx32 camera_position, camera_up, camera_target;
                    u8 unknown_394[8];
                };
            };
            FieldPartyManager *party_manager;
            FieldTimer *timer;
            FieldModelAnimation *model_animation;
            FieldSpriteAnimation *sprite_animation;
            GameSpriteWindowManager *windows;
            FieldAreaContext *areas[2];
        };
    };
} FieldSystem;
typedef char FieldSystem_SizeCheck[sizeof(FieldSystem) == 952 ? 1 : -1];
typedef struct FieldSystemSnapshot {
    FieldPartyManagerSnapshot parties;
    FieldAreaSnapshot areas[2];
} FieldSystemSnapshot;
typedef char FieldSystemSnapshot_SizeCheck[sizeof(FieldSystemSnapshot) == 804 ? 1 : -1];
typedef struct FieldPersistentSystems {
    struct {
        u32 initialized : 1, unknown_01_31 : 31;
    } flags;
    FieldTimer *timer;
    FieldModelAnimation *model_animation;
    FieldPartyManager *party_manager;
} FieldPersistentSystems;
typedef char FieldPersistentSystems_SizeCheck[sizeof(FieldPersistentSystems) == 16 ? 1 : -1];

/* Touch states packed into the field script input word. */
enum FieldTouchState {
    FIELD_TOUCH_DISABLED = 0,
    FIELD_TOUCH_IDLE = 1,
    FIELD_TOUCH_PRESSED = 2,
    FIELD_TOUCH_HELD = 3,
    FIELD_TOUCH_RELEASED = 4
};
#ifdef __cplusplus
extern "C" {
#endif
void FieldSystem_LoadProjection(FieldSystem *field);
void FieldSystem_AdjustMemberHp(FieldSystem *system, int member, int amount);
void FieldSystem_RefillMemberHp(FieldSystem *system, int member);
void FieldSystem_CreateSpriteWindows(FieldSystem *system);
void FieldSystem_ReleaseSharedResources(FieldSystem *system);
FieldSystem *FieldSystem_Init(FieldSystem *system, u32 priority, u32 unused, void *argument);
FieldSystem *FieldSystem_Destroy(FieldSystem *system);
FieldSystem *FieldSystem_Delete(FieldSystem *system);
void FieldSystem_UpdateTouch(FieldSystem *system);
void FieldSystem_SaveSnapshot(FieldSystem *system, FieldSystemSnapshot *snapshot,
                              const FieldAreaContext *first, const FieldAreaContext *second);
#ifdef __cplusplus
}
#endif
#endif
