#include <game/scene_sound.h>

extern "C" {
#include <game/audio.h>

typedef struct SceneSoundManagerLayout {
    u8 unknown_0000[42988];
    SceneSoundTask *sound_tasks[16];
    u8 unknown_a82c[4];
} SceneSoundManagerLayout;
typedef char SceneSoundManagerLayout_SizeCheck[sizeof(SceneSoundManagerLayout) == 43056 ? 1 : -1];
extern SceneSoundManagerLayout *data_ov007_020a6b90;
extern void func_ov005_0206650c(SceneSoundTask *);
extern SceneSoundTask *func_ov005_0206659c(void (*)(SceneSoundTask *), int, int);

int SceneSound_PlayRepeated(int sequence, int delay, int interval, int count)
{
    int repeat_count = count;
    int initial_delay = delay;
    int slot;
    SceneSoundTask *task;
    if (count == 1)
        interval = 0;
    if (delay <= 0 && interval == 0) {
        GameAudio_PlayEffectDelayed(sequence, 0, -1);
        return -1;
    }
    for (slot = 0;; ++slot) {
        if (slot >= 16)
            return -1;
        if (!data_ov007_020a6b90->sound_tasks[slot])
            break;
    }
    task = func_ov005_0206659c(0, 5, 1);
    data_ov007_020a6b90->sound_tasks[slot] = task;
    task->slot = slot;
    task->timer = initial_delay;
    task->sequence = sequence;
    if (interval == 0) {
        task->callback = SceneSoundTask_PlayOnce;
    } else if (interval < 0) {
        task->state.remaining = repeat_count;
        task->callback = SceneSoundTask_BeginMeasuredPlayback;
    } else {
        task->interval = interval;
        task->state.remaining = repeat_count;
        task->callback = SceneSoundTask_UpdatePeriodic;
    }
    return slot;
}

void SceneSoundTask_PlayOnce(SceneSoundTask *task)
{
    if (--task->timer <= 0) {
        GameAudio_PlayEffectDelayed(task->sequence, 0, -1);
        if (data_ov007_020a6b90->sound_tasks[task->slot] == task)
            data_ov007_020a6b90->sound_tasks[task->slot] = 0;
        func_ov005_0206650c(task);
    }
}

void SceneSoundTask_BeginMeasuredPlayback(SceneSoundTask *task)
{
    if (--task->timer <= 0) {
        task->state.handle = GameAudio_PlayEffectOnFreeHandle(task->sequence, -1);
        task->interval = 0;
        task->callback = SceneSoundTask_MeasureInterval;
        u16 remaining = task->state.remaining;
        /* The native counter decrement wraps through u16 before the 14-bit store. */
        if (remaining)
            task->state.remaining = remaining + (u16)-1;
    }
}

void SceneSoundTask_MeasureInterval(SceneSoundTask *task)
{
    ++task->interval;
    if (!((1 << task->state.handle) & GameAudio_GetEffectMask())) {
        GameAudio_PlayEffectDelayed(task->sequence, 0, -1);
        task->timer = task->interval;
        task->callback = SceneSoundTask_UpdatePeriodic;
        if (task->state.remaining) {
            task->state.remaining += (u16)-1;
            if (!task->state.remaining) {
                if (data_ov007_020a6b90->sound_tasks[task->slot] == task)
                    data_ov007_020a6b90->sound_tasks[task->slot] = 0;
                func_ov005_0206650c(task);
            }
        }
    }
}

void SceneSoundTask_UpdatePeriodic(SceneSoundTask *task)
{
    if (--task->timer <= 0) {
        GameAudio_PlayEffectDelayed(task->sequence, 0, -1);
        task->timer = task->interval;
        if (task->state.remaining) {
            task->state.remaining += (u16)-1;
            if (!task->state.remaining) {
                if (data_ov007_020a6b90->sound_tasks[task->slot] == task)
                    data_ov007_020a6b90->sound_tasks[task->slot] = 0;
                func_ov005_0206650c(task);
            }
        }
    }
}

void SceneSound_StopTask(int slot)
{
    if (slot >= 0 && slot < 16) {
        SceneSoundTask *task = data_ov007_020a6b90->sound_tasks[slot];
        if (task) {
            data_ov007_020a6b90->sound_tasks[slot] = 0;
            func_ov005_0206650c(task);
        }
    }
}
}
