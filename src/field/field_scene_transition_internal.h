#ifndef PIT_FIELD_SCENE_TRANSITION_INTERNAL_H
#define PIT_FIELD_SCENE_TRANSITION_INTERNAL_H
#include <game/field_scene_transition.h>
#include <game/battle_entry.h>
#include <game/save_data.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct PersistentTransitionFlags {
    u32 initialized : 1, first : 8, second : 8, side : 1, unknown : 14;
} PersistentTransitionFlags;
extern PersistentTransitionFlags data_0205a01c;
void func_ov000_0206a130(FieldSystem *);
void func_0202cbd4(void *, int, u32);
typedef struct FieldSaveTransitionView {
    u8 prefix[0x558];
    union {
        BattleEntrySaveView request;
        struct {
            u8 prefix[6];
            union {
                u16 raw;
                struct {
                    u16 mode : 15, side : 1;
                };
            } exit;
            u8 flags_low;
            struct {
                u8 side : 1, unknown : 7;
            } flags_high;
        } bits;
    };
} FieldSaveTransitionView;
#define SAVE ((FieldSaveTransitionView *)gSaveData)
#define REQUEST (&SAVE->request)

static inline int IsFadeActive(FieldSystem *system) {
    return system->areas[0]->unknown_2402.unknown_00 || system->areas[1]->unknown_2402.unknown_00;
}

static inline void FadeBoth(FieldSystem *system) {
    FieldArea_StartBrightness(system->areas[0], 255, -16, 16);
    FieldArea_StartBrightness(system->areas[1], 255, -16, 16);
}

static inline void SuspendAreaTasks(FieldSystem *system) {
    GameTask_Disable((GameTask *)system->areas[0]->navigation_changes);
    GameTask_Disable((GameTask *)system->areas[1]->navigation_changes);
}

static inline void StorePersistentTransition(FieldSystem *system) {
    func_ov000_0206a130(system);
    data_0205a01c.first = 47;
    data_0205a01c.second = 70;
    data_0205a01c.side = SAVE->bits.exit.side;
}

/* Separate native entry points serve the corresponding scene phases. */
void FieldSystem_ReleaseForGameOver(FieldSystem *system);
int FieldSystem_IsGameOverFadeActive(FieldSystem *system);
void FieldSystem_ReleaseForCredits(FieldSystem *system);
int FieldSystem_IsCreditsFadeActive(FieldSystem *system);
void FieldSystem_PrepareShopScene(FieldSystem *system);
int FieldSystem_IsShopFadeActive(FieldSystem *system);
void FieldSystem_PrepareSaveScene(FieldSystem *system);
int FieldSystem_IsSaveFadeActive(FieldSystem *system);
void FieldSystem_PreparePauseScene(FieldSystem *system);
int FieldSystem_IsPauseFadeActive(FieldSystem *system);
void func_ov000_0206b1fc(FieldSystem *);
int func_ov000_0206b3a0(FieldSystem *);
void GameAudio_StopEffects(void);
typedef struct FieldIrqTransitionView {
    u8 prefix[40];
    struct {
        u32 frozen : 1, unknown : 31;
    } flags;
} FieldIrqTransitionView;
static inline void FreezeFieldIrq(FieldSystem *system) {
    ((FieldIrqTransitionView *)system->irq_task)->flags.frozen = 1;
}
#ifdef __cplusplus
}
#endif
#endif
