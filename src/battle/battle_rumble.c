/* Four-slot Rumble Pak scheduling (overlay 2, 0x02065DFC..0x02066004).
 * A positive delay counts down to zero; playback starts on the following update.
 * Delayed amounts are stored as signed halfwords, unlike immediate calls. */

#include <game/rumble.h>
#include <game/battle_effect.h>
#include <game/battle_attack_loader.h>

typedef struct BattleRumbleTask BattleRumbleTask;
struct BattleRumbleTask {
    int delay;
    s16 duration;
    u8 repeated, pattern;
    void (*update)(BattleRumbleTask *);
};
typedef struct BattleRumbleWork {
    u8 prefix[70484];
    BattleRumbleTask tasks[4];
} BattleRumbleWork;
typedef char BattleRumbleTask_SizeCheck[sizeof(BattleRumbleTask) == 12 ? 1 : -1];
/* The final indexed view also stays inside the 70,976-byte common allocation. */
typedef char BattleRumbleWork_BoundsCheck[sizeof(BattleRumbleWork) + 3 * 12 <= 70976 ? 1 : -1];
#define WORK ((BattleRumbleWork *)data_ov002_020c0660)
void BattleRumble_UpdateDelayed(BattleRumbleTask *);
int BattleRumble_FindFreeDelaySlot(void);

int BattleRumble_FindFreeDelaySlot(void)
{
    int i = 0;
    BattleRumbleWork *indexed = WORK;
    do {
        if (!indexed->tasks[0].update) return i;
        ++i;
        indexed = (BattleRumbleWork *)((u8 *)indexed + 12);
    } while (i < 4);
    return -1;
}

void BattleRumble_UpdateDelayed(BattleRumbleTask *task)
{
    if (task->delay > 0) --task->delay;
    else {
        switch (task->repeated) {
        case 0: BattleRumble_PlayTimed(task->pattern + 1, task->duration, 0); break;
        case 1: BattleRumble_PlayRepeated(task->pattern + 1, task->duration, 0); break;
        }
        task->update = 0;
    }
}

void BattleRumble_PlayTimed(int pattern, int duration, int delay)
{
    if (pattern - 1 < 0) return;
    if (delay) {
        int index = BattleRumble_FindFreeDelaySlot();
        BattleRumbleTask *task;
        if (index == -1) return;
        task = &WORK->tasks[index];
        task->pattern = pattern - 1;
        task->update = BattleRumble_UpdateDelayed;
        task->delay = delay;
        task->repeated = 0;
        task->duration = duration;
    } else GameRumble_PlayTimed(pattern - 1, duration);
}

void BattleRumble_PlayRepeated(int pattern, int duration, int delay)
{
    if (pattern - 1 < 0) return;
    if (delay) {
        int index = BattleRumble_FindFreeDelaySlot();
        BattleRumbleTask *task;
        if (index == -1) return;
        task = &WORK->tasks[index];
        task->pattern = pattern - 1;
        task->update = BattleRumble_UpdateDelayed;
        task->delay = delay;
        task->repeated = 1;
        task->duration = duration;
    } else GameRumble_PlayRepeated(pattern - 1, duration);
}

/* Stopping playback does not discard queued requests. */
extern u8 data_0205a00c;
void BattleRumble_Stop(void)
{
    if (data_0205a00c) GameRumble_Stop();
}
