#ifndef PIT_GAME_BATTLE_SELECTION_ANIMATION_H
#define PIT_GAME_BATTLE_SELECTION_ANIMATION_H
#include <game/battle_scene.h>
enum BattleCommandWheelConstant {
    BATTLE_COMMAND_WHEEL_OFFSET = 0x6530,
    BATTLE_COMMAND_WHEEL_ENTRIES_OFFSET = 0x6538,
    BATTLE_TARGET_CURSOR_ANIMATION_OFFSET = 0x6574,
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_SELECTED_COMMAND_OFFSET = 0x11A,
    BATTLE_INTERFACE_ANIMATION_COUNT_OFFSET = 0x12E,
    BATTLE_COMMAND_WHEEL_MAX_ENTRIES = 5,
    BATTLE_COMMAND_WHEEL_OPEN_RADIUS = 0x2400,
    BATTLE_COMMAND_WHEEL_RADIUS_STEP = 0x480,
    BATTLE_COMMAND_WHEEL_ENTRY_LIFT = 0x400,
    BATTLE_COMMAND_WHEEL_ENTRY_LIFT_STEP = 0x66,
    BATTLE_COMMAND_WHEEL_BOUNCE_FRAMES = 12,
    BATTLE_COMMAND_WHEEL_SORT_CAPACITY = 8,
    BATTLE_COMMAND_WHEEL_RENDER_RESOURCE_ID = 3,
    BATTLE_COMMAND_WHEEL_FULL_INTENSITY = 31
};

enum BattleCommandWheelPhase {
    BATTLE_COMMAND_WHEEL_PHASE_ATTACH = 1,
    BATTLE_COMMAND_WHEEL_PHASE_OPEN = 2,
    BATTLE_COMMAND_WHEEL_PHASE_ACTIVE = 3
};

typedef union BattleCommandWheelEntryFlags {
    u16 raw;
    struct {
        s16 icon_id : 15;
        u16 dismiss_after_bounce : 1;
    } bits;
} BattleCommandWheelEntryFlags;

typedef struct BattleCommandWheelEntry {
    s16 angle;
    s16 lift;
    s8 spin_angle;
    s8 bounce_timer;
    BattleCommandWheelEntryFlags flags;
} BattleCommandWheelEntry;

typedef union BattleCommandWheelFlags {
    u16 raw;
    struct {
        u16 moving : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleCommandWheelFlags;

typedef struct BattleCommandWheelState {
    BattleSceneObject *scene_object;
    s16 intensity;
    s16 phase;
    BattleCommandWheelEntry entries[BATTLE_COMMAND_WHEEL_MAX_ENTRIES];
    s16 entry_count;
    s16 angular_speed;
    s16 radius;
    BattleCommandWheelFlags flags;
} BattleCommandWheelState;

enum BattleTargetCursorConstant {
    BATTLE_TARGET_CURSOR_OFFSET = 0x6568,
    BATTLE_INTERFACE_BLOCKED_OFFSET = 0x12E,
    BATTLE_TARGET_CURSOR_FRAME = 17,
    BATTLE_TARGET_CURSOR_BOUNCE_FRAMES = 12
};

typedef union BattleTargetCursorAnimation {
    u16 raw;
    struct {
        s8 bounce_timer;
        u8 flags;
    } bytes;
    struct {
        u16 unknown_00_07 : 8;
        u16 uniform_targets : 1;
        u16 unknown_09_15 : 7;
    } bits;
} BattleTargetCursorAnimation;

typedef struct BattleTargetCursorState {
    BattleSceneObject *focus_object;
    s16 intensity;
    s16 visible;
    s16 unknown_08;
    s16 rotation_angle;
    BattleTargetCursorAnimation animation;
} BattleTargetCursorState;


typedef char BattleCommandWheelEntry_SizeCheck[sizeof(BattleCommandWheelEntry) == 8 ? 1 : -1];
typedef char BattleCommandWheelState_SizeCheck[sizeof(BattleCommandWheelState) == 56 ? 1 : -1];
typedef char BattleTargetCursorState_SizeCheck[sizeof(BattleTargetCursorState) == 16 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
int BattleCommandWheel_TriggerEntryBounce(int dismiss_after_bounce);
int BattleTargetCursor_TriggerBounce(void);
void BattleTargetCursor_Update(BattleTargetCursorState *state);
#ifdef __cplusplus
}
#endif
#endif
