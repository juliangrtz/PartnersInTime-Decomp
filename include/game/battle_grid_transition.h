#ifndef PIT_GAME_BATTLE_GRID_TRANSITION_H
#define PIT_GAME_BATTLE_GRID_TRANSITION_H

#include <nitro.h>

typedef struct BattleGridTransitionState {
    u16 angle;
    u16 velocity;
    u16 remaining_frames;
} BattleGridTransitionState;

typedef struct BattleGridTransitionTask {
    struct BattleGridTransitionTask *next;
    void (*callback)(struct BattleGridTransitionTask *task);
    struct BattleGridTransitionTask **owner_slot;
    BattleGridTransitionState state;
} BattleGridTransitionTask;

typedef char BattleGridTransitionState_SizeCheck[
    sizeof(BattleGridTransitionState) == 6 ? 1 : -1];
typedef char BattleGridTransitionTask_SizeCheck[
    sizeof(BattleGridTransitionTask) == 0x14 ? 1 : -1];

typedef struct BattleGridCaptureTask BattleGridCaptureTask;
typedef struct BattleGridCaptureSourceTask BattleGridCaptureSourceTask;

typedef struct BattleGridCaptureState {
    BattleGridCaptureSourceTask *source_task;
    u16 angle;
    u16 velocity;
    u16 started;
    u16 padding_0A;
} BattleGridCaptureState;

struct BattleGridCaptureTask {
    BattleGridCaptureTask *next;
    void (*callback)(BattleGridCaptureTask *task);
    BattleGridCaptureTask **owner_slot;
    BattleGridCaptureState state;
};

typedef struct BattleGridCaptureSourceState {
    BattleGridCaptureTask *capture_task;
    u16 angle;
    u16 velocity;
    u16 remaining_frames;
    u16 padding_0A;
} BattleGridCaptureSourceState;

struct BattleGridCaptureSourceTask {
    BattleGridCaptureSourceTask *next;
    void (*callback)(BattleGridCaptureSourceTask *task);
    BattleGridCaptureSourceTask **owner_slot;
    BattleGridCaptureSourceState state;
};

typedef char BattleGridCaptureState_SizeCheck[
    sizeof(BattleGridCaptureState) == 0x0C ? 1 : -1];
typedef char BattleGridCaptureTask_SizeCheck[
    sizeof(BattleGridCaptureTask) == 0x18 ? 1 : -1];
typedef char BattleGridCaptureSourceState_SizeCheck[
    sizeof(BattleGridCaptureSourceState) == 0x0C ? 1 : -1];
typedef char BattleGridCaptureSourceTask_SizeCheck[
    sizeof(BattleGridCaptureSourceTask) == 0x18 ? 1 : -1];

BattleGridTransitionTask *BattleGridTransition_InitializeTask(
    BattleGridTransitionTask *task);
BattleGridCaptureTask *BattleGridCapture_InitializeTask(
    BattleGridCaptureTask *task);
BattleGridCaptureTask *BattleGridCaptureSource_InitializeTask(
    BattleGridCaptureSourceTask *task);

#endif
