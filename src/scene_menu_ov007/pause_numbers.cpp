/*
 * Pause numeric readouts (overlay 7, 0x02080D4C-0x02081334).
 *
 * The interpolated numbers on the status screen - level, HP, coins, play time -
 * the blinking clock separator, and the low-HP warning icon. Interpolated
 * because a value counts up to its target rather than jumping.
 */

#include "pause_scene_internal.h"
#include <game/pause_numbers.h>
#include <game/pause_clock.h>

/* Prefix of the live save context through the 60 Hz play-time counter. */
struct PauseNumberSaveView {
    u8 unknown_000[1016];
    SavePartyMember members[4];
    u32 coins;
    u8 unknown_48c[140];
    u32 play_frames;
};
typedef char PauseNumberSaveViewSize[sizeof(PauseNumberSaveView) == 1308 ? 1 : -1];

extern "C" {
void func_ov007_02081334(int, int, int, int, int, int, int);
void func_ov007_020814e4(int, int, int, int);
}

extern "C" void PauseNumber_Update(PauseNumberTask *task)
{
    int value, advance;
    int smooth_updates = 8, glyph = 88;
    int padding = 0;
    switch (task->kind) {
    case PAUSE_NUMBER_LEVEL:
        advance = 8;
        value = ((PauseNumberSaveView *)gSaveData)->members[task->member].experience.fields.level;
        padding = 3;
        break;
    case PAUSE_NUMBER_CURRENT_HP:
        advance = 8;
        value = ((PauseNumberSaveView *)gSaveData)->members[task->member].current_hp;
        padding = 3;
        break;
    case PAUSE_NUMBER_MAX_HP:
        advance = 8;
        value = ((PauseNumberSaveView *)gSaveData)->members[task->member].max_hp;
        padding = 3;
        break;
    case PAUSE_NUMBER_COINS:
        advance = 8;
        padding = 3;
        value = ((PauseNumberSaveView *)gSaveData)->coins;
        break;
    case PAUSE_NUMBER_HOURS:
        padding = 1;
        advance = 8;
        smooth_updates = 1;
        value = ((PauseNumberSaveView *)gSaveData)->play_frames / 216000u;
        glyph = 168;
        break;
    case PAUSE_NUMBER_MINUTES:
        value = ((PauseNumberSaveView *)gSaveData)->play_frames;
        if ((u32)value >= 21596400u)
            value = 59;
        else {
            value = (u32)value % 216000u / 3600u;
            if (value > 59)
                value = 59;
        }
        padding = 1;
        smooth_updates = 1;
        advance = 8;
        glyph = 168;
        break;
    }
    switch (task->state) {
    case PAUSE_NUMBER_INITIALIZE:
        task->current_q8 = value << 8;
        task->target_q8 = task->current_q8;
        task->state = PAUSE_NUMBER_IDLE;
        break;
    case PAUSE_NUMBER_IDLE:
        if (task->current_q8 == value << 8)
            break;
        ++task->state;
        /* Fall through to the first interpolation update. */
    case PAUSE_NUMBER_INTERPOLATE:
        if (task->target_q8 != value << 8) {
            task->target_q8 = value << 8;
            task->step_q8 = (task->target_q8 - task->current_q8) / smooth_updates;
        }
        task->current_q8 += task->step_q8;
        if (task->step_q8 < 0) {
            if (task->current_q8 <= task->target_q8) {
                task->current_q8 = task->target_q8;
                --task->state;
            }
        } else {
            if (task->current_q8 >= task->target_q8) {
                task->current_q8 = task->target_q8;
                --task->state;
            }
        }
        break;
    }
    value = task->current_q8 / 256;
    if (task->last_value != value) {
        task->last_value = value;
        u32 limit = 10 * task->divisor;
        if ((u32)value >= limit)
            value = limit - 1;
        func_ov007_020814e4(task->x, task->y, padding + task->digits * advance, 16);
        int x = task->x, y = task->y;
        int divisor = task->divisor;
        int visible = task->leading_zeroes;
        for (int digit = 0; digit < task->digits; ++digit) {
            int number = value / divisor;
            if (number || divisor == 1 || visible) {
                visible = 1;
                func_ov007_02081334(x, y, glyph + 8 * number, 2, 2, task->palette, 1);
            }
            x += advance;
            value %= divisor;
            divisor /= 10;
        }
    }
}

extern "C" void PauseClock_UpdateSeparator(PauseClockSeparatorTask *task)
{
    if (++task->timer >= 30) {
        task->timer -= 30;
        task->visible ^= 1;
        if (task->visible)
            func_ov007_02081334(72, 161, 268, 1, 2, 48, 1);
        else
            func_ov007_020814e4(74, 165, 4, 8);
    }
}

/* kind is 0..5; the native switch does not initialize unsupported kinds. */
extern "C" PauseNumberTask *PauseNumber_Create(int x, int y, int member, int kind, int digits,
                                              int leading_zeroes, int group)
{
    PauseNumberTask *task = (PauseNumberTask *)func_ov005_0206659c(
        (void (*)(PauseMenuElement *))PauseNumber_Update, (u8)group, 1);
    task->member = (u8)member;
    task->kind = kind;
    u8 palette;
    switch (task->kind) {
    case PAUSE_NUMBER_LEVEL:
    case PAUSE_NUMBER_CURRENT_HP:
    case PAUSE_NUMBER_MAX_HP:
        palette = 2;
        break;
    case PAUSE_NUMBER_COINS:
        palette = 2;
        break;
    case PAUSE_NUMBER_HOURS:
    case PAUSE_NUMBER_MINUTES:
        palette = 3;
        break;
    }
    task->palette = 16 * palette;
    task->x = x;
    task->y = y;
    task->divisor = 1;
    for (int digit = 0; digit < digits - 1; ++digit)
        task->divisor *= 10;
    task->digits = digits;
    task->leading_zeroes = (u8)leading_zeroes;
    task->last_value = -1;
    return task;
}

#include <game/pause_hp_warning.h>
#include <game/battle_scene.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(PauseHpWarningTask *);
void func_ov005_0206650c(PauseHpWarningTask *);
void func_ov005_02069084(void *, int);
}

extern "C" void PauseHpWarning_Update(PauseHpWarningTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    if (WORK.hp_warning_mode == PAUSE_HP_WARNING_DISABLED) {
        func_ov005_0206650c(task);
        return;
    }
    if (WORK.hp_warning_mode == PAUSE_HP_WARNING_SUSPENDED)
        return;
    SavePartyMember *member = (SavePartyMember *)(gSaveData + 0x3f8) + task->member;
    if (100 * member->current_hp > 25 * member->max_hp) {
        sprite->animation_offset_x = -256;
        sprite->animation_offset_y = -256;
    } else {
        int y = task->y;
        sprite->animation_offset_x = task->x;
        sprite->animation_offset_y = y;
    }
    func_ov005_02069084(sprite, 60);
}
