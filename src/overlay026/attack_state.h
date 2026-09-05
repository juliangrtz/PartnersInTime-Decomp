#ifndef PARTNERS_IN_TIME_OVERLAY026_ATTACK_STATE_H
#define PARTNERS_IN_TIME_OVERLAY026_ATTACK_STATE_H

#include <nitro.h>

enum Overlay26AttackConstant {
    OVERLAY26_TRACKED_OBJECT_COUNT = 4,
    OVERLAY26_FIRST_TRACKED_OBJECT_ID = 56,
    OVERLAY26_ASYNC_TASK_COUNT = 2,
};

typedef struct Overlay26TrackedObjectState {
    u8 unknown_00[5];
    union {
        u8 flags;
        struct {
            u8 unknown_flag_0 : 1;
            u8 restore_actor : 1;
            u8 unknown_flags_2_7 : 6;
        } flag_bits;
    };
    u8 unknown_06[2];
    u16 object_id;
    s16 state;
} Overlay26TrackedObjectState;

typedef struct Overlay26AttackState {
    Overlay26TrackedObjectState tracked_objects[
        OVERLAY26_TRACKED_OBJECT_COUNT];
    u8 unknown_30[4];
    void *primary_async_task;
    void *async_tasks[OVERLAY26_ASYNC_TASK_COUNT];
    void **completion_link;
    u16 resource_id;
} Overlay26AttackState;

typedef struct Overlay26ActionStateView {
    u8 unknown_00[0x38];
    s8 state;
} Overlay26ActionStateView;

typedef struct Overlay26AttackOwnerView {
    u8 unknown_00[0x78];
    void *attack_state;
} Overlay26AttackOwnerView;

typedef char Overlay26TrackedObjectState_SizeCheck[
    sizeof(Overlay26TrackedObjectState) == 0xC ? 1 : -1];
typedef char Overlay26AttackState_SizeCheck[
    sizeof(Overlay26AttackState) == 0x48 ? 1 : -1];

extern Overlay26AttackState *gOverlay26AttackState;

#endif
