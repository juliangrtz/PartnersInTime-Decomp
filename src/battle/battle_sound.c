#include <game/battle_sound.h>
#include <nitro.h>
#include <game/audio.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
typedef struct RepeatSound {
    s16 sound, timer, interval;
    union {
        u16 raw;
        struct {
            u16 remaining : 14, handle : 2;
        } bits;
    } flags;
} RepeatSound;
typedef struct SoundTask SoundTask;
struct SoundTask {
    SoundTask *next;
    void (*update)(SoundTask *);
    SoundTask **owner;
    RepeatSound sound;
};
void BattleSound_UpdateRepeat(SoundTask *task);
void BattleSound_WaitForInitialEffect(SoundTask *task);
void BattleSound_StartMeasuredRepeat(SoundTask *task);
void BattleSound_UpdateDelayed(SoundTask *task);

typedef struct SoundContextView {
    u8 prefix[51380];
    SoundTask *slots[16];
} SoundContextView;
typedef char SoundTaskSizeCheck[sizeof(SoundTask) == 20 ? 1 : -1];
typedef char RepeatSoundSizeCheck[sizeof(RepeatSound) == 8 ? 1 : -1];
#define SOUND_SLOT(i) (((SoundContextView *)gBattleContext)->slots[i])
int BattleSound_Play(int sound_id, int delay, int interval, int count) {
    if (count == 1)
        interval = 0;
    if (delay <= 0 && !interval) {
        GameAudio_PlayEncoded(sound_id);
        return -1;
    }
    {
        int slot;
        SoundTask *task;
        RepeatSound *sound;
        for (slot = 0;; ++slot) {
            if (slot >= 16)
                return -1;
            if (!SOUND_SLOT(slot))
                break;
        }
        task = (SoundTask *)BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 51052), 0);
        sound = &task->sound;
        SOUND_SLOT(slot) = task;
        task->owner = &SOUND_SLOT(slot);
        sound->timer = delay;
        sound->sound = sound_id;
        if (!interval) {
            task->update = BattleSound_UpdateDelayed;
        } else if (interval < 0) {
            sound->flags.bits.remaining = (u16)count;
            task->update = BattleSound_StartMeasuredRepeat;
        } else {
            sound->interval = interval;
            sound->flags.bits.remaining = (u16)count;
            task->update = BattleSound_UpdateRepeat;
        }
        return slot;
    }
}

void BattleSound_UpdateDelayed(SoundTask *task) {
    RepeatSound *sound = &task->sound;
    if (--sound->timer <= 0) {
        GameAudio_PlayEncoded(sound->sound);
        task->update = 0;
    }
}
void BattleSound_StartMeasuredRepeat(SoundTask *task) {
    RepeatSound *sound = &task->sound;
    if (--sound->timer <= 0) {
        sound->flags.bits.handle = (u16)GameAudio_PlayEffectOnFreeHandle(sound->sound, -1);
        sound->interval = 0;
        task->update = BattleSound_WaitForInitialEffect;
        if (sound->flags.bits.remaining) {
            /* The native decrement wraps in 16 bits before updating the 14-bit field. */
            sound->flags.bits.remaining = (u16)(sound->flags.bits.remaining + (u16)-1);
        }
    }
}
void BattleSound_WaitForInitialEffect(SoundTask *task) {
    RepeatSound *sound = &task->sound;
    ++sound->interval;
    if (!((1 << sound->flags.bits.handle) & GameAudio_GetEffectMask())) {
        GameAudio_PlayEncoded(sound->sound);
        sound->timer = sound->interval;
        task->update = BattleSound_UpdateRepeat;
        if (sound->flags.bits.remaining) {
            /* The native decrement wraps in 16 bits before updating the 14-bit field. */
            sound->flags.bits.remaining = (u16)(sound->flags.bits.remaining + (u16)-1);
            if (!sound->flags.bits.remaining)
                task->update = 0;
        }
    }
}
void BattleSound_UpdateRepeat(SoundTask *task) {
    RepeatSound *sound = &task->sound;
    if (--sound->timer <= 0) {
        GameAudio_PlayEncoded(sound->sound);
        sound->timer = sound->interval;
        if (sound->flags.bits.remaining) {
            /* The native decrement wraps in 16 bits before updating the 14-bit field. */
            sound->flags.bits.remaining = (u16)(sound->flags.bits.remaining + (u16)-1);
            if (!sound->flags.bits.remaining)
                task->update = 0;
        }
    }
}

void BattleSound_Stop(int slot) {
    if (slot < 0 || slot >= 16)
        return;
    if (SOUND_SLOT(slot))
        SOUND_SLOT(slot)->update = 0;
}
