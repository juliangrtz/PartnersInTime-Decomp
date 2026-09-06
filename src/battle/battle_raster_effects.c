#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_raster_effects.h>

enum BattleRasterEffectOffset {
    BATTLE_RASTER_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_RASTER_VIEW_X_OFFSET = 0xCB9C,
    BATTLE_RASTER_VIEW_Y_OFFSET = 0xCB9E,
    BATTLE_RASTER_STATE_A_OFFSET = 0xCBCC,
    BATTLE_RASTER_STATE_B_OFFSET = 0xCBD0,
    BATTLE_RASTER_TABLE_A_OFFSET = 0x61808,
    BATTLE_RASTER_TABLE_B_OFFSET = 0x61C08
};

enum BattleRasterEffectConstant {
    BATTLE_RASTER_OBJECT_DATA_ID = 51,
    BATTLE_RASTER_MODE_DIRECT = 0xFFFF
};

typedef struct BattleRasterEffectState {
    s16 mode;
    s16 value;
    u16 extent;
    u16 duration;
    u16 progress;
    u16 progress_limit;
} BattleRasterEffectState;

struct BattleRasterEffectTask {
    struct BattleRasterEffectTask *next;
    void (*callback)(struct BattleRasterEffectTask *task);
    struct BattleRasterEffectTask **owner_slot;
    BattleRasterEffectState state;
};

typedef char BattleRasterEffectState_SizeCheck[
    sizeof(BattleRasterEffectState) == 0x0C ? 1 : -1];
typedef char BattleRasterEffectTask_SizeCheck[
    sizeof(BattleRasterEffectTask) == 0x18 ? 1 : -1];

extern void *func_ov002_020725a4(
    void (*callback)(void *task), void *argument_1,
    int argument_2, int argument_3);
extern void func_ov002_0208b254(void *task);
extern void func_ov002_0208ace0(
    BattleObjectDataLoadState *load_state, s16 value);
extern int func_0202da0c(
    int value, int extent, int duration, int progress,
    int table_cursor, int scale, int table, int flags);
extern int func_ov002_020b05f0(s16 x, s16 y, int frame);
extern int BattleRasterEffect_ApplyToTable(
    int value, int extent, int duration, int progress,
    void *table, int *table_cursor);
extern int BattleRasterEffect_ApplyToBothTables(
    int value, int extent, int duration, int progress);

void BattleEffectParticle_Update(BattleRasterEffectTask *task);
void BattleRasterResourceTransition_UpdateIn(BattleRasterEffectTask *task);
void BattleRasterResourceTransition_UpdateOut(BattleRasterEffectTask *task);
int BattleRasterEffect_UpdatePersistent(BattleRasterEffectTask *task);
int BattleRasterEffect_UpdateFinite(BattleRasterEffectTask *task);

/* Metrowerks emits C functions in reverse source order. */
int BattleRasterEffect_ApplyToBothTables(
    int value, int extent, int duration, int progress) {
    BattleRasterEffect_ApplyToTable(
        value, extent, duration, progress,
        gBattleContext + BATTLE_RASTER_TABLE_A_OFFSET,
        (int *)(gBattleContext + BATTLE_RASTER_STATE_A_OFFSET));
    return BattleRasterEffect_ApplyToTable(
        value, extent, duration, progress,
        gBattleContext + BATTLE_RASTER_TABLE_B_OFFSET,
        (int *)(gBattleContext + BATTLE_RASTER_STATE_B_OFFSET));
}

int BattleRasterEffect_ApplyToTable(
    int value, int extent, int duration, int progress,
    void *table, int *table_cursor) {
    progress <<= 8;
    if (*table_cursor != 0) {
        table = 0;
    } else {
        *table_cursor = (int)table + 0x200;
    }
    return func_0202da0c(
        value, extent, duration, progress,
        *table_cursor, 0x100, (int)table, 0);
}

BattleRasterEffectTask *BattleRasterEffect_StartFinite(
    int value, u16 extent, int duration, int progress_limit, int mode) {
    BattleRasterEffectTask *task =
        (BattleRasterEffectTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_RASTER_TASK_POOL_OFFSET),
            0);
    BattleRasterEffectState *state = &task->state;

    task->callback =
        (void (*)(BattleRasterEffectTask *))BattleRasterEffect_UpdateFinite;
    task->state.mode = mode;
    state->value = value;
    state->extent = extent;
    state->duration = duration;
    state->progress = 0;
    state->progress_limit = progress_limit;
    return task;
}

int BattleRasterEffect_UpdateFinite(BattleRasterEffectTask *task) {
    BattleRasterEffectState *state = &task->state;

    state->progress = (u16)state->progress + 1;
    if (state->progress_limit <= (u16)state->progress) {
        state->progress = state->progress_limit;
        task->callback = 0;
    }

    /* Forward labels preserve MWCC's original case-block order. */
    if ((u16)state->mode == 0) {
        goto render_table_a;
    }
    if ((u16)state->mode == 1) {
        goto render_table_b;
    }
    if ((u16)state->mode != BATTLE_RASTER_MODE_DIRECT) {
        return BATTLE_RASTER_MODE_DIRECT;
    }
    return BattleRasterEffect_ApplyToBothTables(
        state->value, state->extent, state->duration, state->progress);

render_table_a:
    return BattleRasterEffect_ApplyToTable(
        state->value, state->extent, state->duration, state->progress,
        gBattleContext + BATTLE_RASTER_TABLE_A_OFFSET,
        (int *)(gBattleContext + BATTLE_RASTER_STATE_A_OFFSET));

render_table_b:
    return BattleRasterEffect_ApplyToTable(
        state->value, state->extent, state->duration, state->progress,
        gBattleContext + BATTLE_RASTER_TABLE_B_OFFSET,
        (int *)(gBattleContext + BATTLE_RASTER_STATE_B_OFFSET));
}

BattleRasterEffectTask *BattleRasterEffect_StartPersistent(
    s16 value, u16 extent, u16 duration, s16 progress_limit, int mode) {
    BattleRasterEffectTask *task =
        (BattleRasterEffectTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_RASTER_TASK_POOL_OFFSET),
            0);
    BattleRasterEffectState *state = &task->state;

    task->callback =
        (void (*)(BattleRasterEffectTask *))BattleRasterEffect_UpdatePersistent;
    task->state.mode = mode;
    state->value = value;
    state->extent = extent;
    state->duration = duration;
    state->progress = 0;
    state->progress_limit = progress_limit;
    return task;
}

int BattleRasterEffect_UpdatePersistent(BattleRasterEffectTask *task) {
    BattleRasterEffectState *state = &task->state;

    state->progress = (u16)state->progress + 1;
    if (state->progress_limit <= (u16)state->progress) {
        state->progress = state->progress_limit;
    }

    /* Forward labels preserve MWCC's original case-block order. */
    if ((u16)state->mode == 0) {
        goto render_table_a;
    }
    if ((u16)state->mode == 1) {
        goto render_table_b;
    }
    if ((u16)state->mode != BATTLE_RASTER_MODE_DIRECT) {
        return BATTLE_RASTER_MODE_DIRECT;
    }
    return BattleRasterEffect_ApplyToBothTables(
        state->value, state->extent, state->duration, state->progress);

render_table_a:
    return BattleRasterEffect_ApplyToTable(
        state->value, state->extent, state->duration, state->progress,
        gBattleContext + BATTLE_RASTER_TABLE_A_OFFSET,
        (int *)(gBattleContext + BATTLE_RASTER_STATE_A_OFFSET));

render_table_b:
    return BattleRasterEffect_ApplyToTable(
        state->value, state->extent, state->duration, state->progress,
        gBattleContext + BATTLE_RASTER_TABLE_B_OFFSET,
        (int *)(gBattleContext + BATTLE_RASTER_STATE_B_OFFSET));
}

BattleRasterEffectTask *BattleRasterResourceTransition_Start(
    int resource_mode, u16 extent, int fade_out_duration,
    int fade_in_duration) {
    BattleRasterEffectTask *task =
        (BattleRasterEffectTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_RASTER_TASK_POOL_OFFSET),
            0);
    BattleRasterEffectState *state = &task->state;

    task->state.mode = resource_mode;
    state->value = extent;
    state->extent = fade_out_duration;
    state->duration = fade_in_duration;
    state->progress = 0;
    task->callback = BattleRasterResourceTransition_UpdateOut;
    return task;
}

void BattleRasterResourceTransition_UpdateOut(BattleRasterEffectTask *task) {
    BattleRasterEffectState *state = &task->state;
    int progress = (s16)state->progress;
    int duration = (s16)state->extent;

    if (progress < duration) {
        state->progress = progress + 1;
    }
    BattleRasterEffect_ApplyToBothTables(
        -4, (u16)state->value, (s16)state->extent,
        (s16)state->progress);
    if ((s16)state->progress >= (s16)state->extent) {
        state->progress = 0;
        func_ov002_0208ace0(
            BattleObjectData_GetLoadState(BATTLE_RASTER_OBJECT_DATA_ID),
            state->mode);
        task->callback = BattleRasterResourceTransition_UpdateIn;
    }
}

void BattleRasterResourceTransition_UpdateIn(BattleRasterEffectTask *task) {
    BattleRasterEffectState *state = &task->state;

    if (BattleObjectData_IsLoadPending(BATTLE_RASTER_OBJECT_DATA_ID) == 0) {
        int progress = (s16)state->progress;
        int duration = (s16)state->duration;

        if (progress < duration) {
            state->progress = progress + 1;
        }
        BattleRasterEffect_ApplyToBothTables(
            4, (u16)state->value, (s16)state->duration,
            (s16)state->progress);
        if ((s16)state->progress >= (s16)state->duration) {
            func_ov002_020725a4(func_ov002_0208b254, 0, 0, 0);
            task->callback = 0;
        }
    }
}

BattleRasterEffectTask *BattleEffectParticle_Spawn(
    int x, int y, int velocity) {
    BattleRasterEffectTask *task =
        (BattleRasterEffectTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_RASTER_TASK_POOL_OFFSET),
            0);

    BattleRasterEffectState *state = &task->state;

    task->callback = BattleEffectParticle_Update;
    state->mode = x;
    state->value = y;
    state->extent = 0;
    state->duration = velocity;
    state->progress = 4;
    return task;
}

void BattleEffectParticle_Update(BattleRasterEffectTask *task) {
    BattleRasterEffectState *state = &task->state;
    int frame = (s16)state->progress;

    if (frame < 14) {
        state->progress = frame + 1;
    } else if (frame < 34) {
        state->progress = frame + 2;
    } else if (frame < 84) {
        state->progress = frame + 5;
    } else if (frame < 164) {
        state->progress = frame + 8;
    } else {
        state->extent = (s16)state->extent + (s16)state->duration;
        frame = (s16)state->progress;
        if (frame < 300) {
            state->progress = frame + 9;
        } else {
            state->progress = frame + 4;
        }
        state->mode += (s16)state->extent / 256;
    }

    func_ov002_020b05f0(
        (s16)(state->mode -
              *(s16 *)(gBattleContext + BATTLE_RASTER_VIEW_X_OFFSET)),
        (s16)(state->value -
              *(s16 *)(gBattleContext + BATTLE_RASTER_VIEW_Y_OFFSET)),
        (s16)state->progress);
    if (state->mode < -20) {
        task->callback = 0;
    }
}
