/*
 * Party status bitmap (overlay 7, 0x02078F5C-0x0207952C).
 *
 * The bitmap the party's status is drawn into, rebuilt when a value changes and
 * cross-faded to the new version so the change is not a jump.
 */

#include "pause_scene_internal.h"
#include <game/pause_party_bitmap.h>

/* Prefix of the live save context through its four existing member records. */
struct PausePartySaveView {
    u8 unknown_000[1016];
    SavePartyMember members[4];
};
typedef char PausePartySaveViewSize[sizeof(PausePartySaveView) == 1160 ? 1 : -1];

extern "C" {
extern u8 data_ov007_0208db50[6];
void func_ov007_0207952c(int, int, int, int, int, int, int, int);
void MIi_CpuClear16(u16, void *, u32);
}

extern "C" void PausePartyBitmap_DrawValue(int x, int y, int member, int kind, int digits, int leading_zeroes)
{
    int value;
    switch (kind) {
    case PAUSE_PARTY_VALUE_LEVEL:
        value = ((PausePartySaveView *)gSaveData)->members[member].experience.fields.level;
        break;
    case PAUSE_PARTY_VALUE_CURRENT_HP:
        value = ((PausePartySaveView *)gSaveData)->members[member].current_hp;
        break;
    case PAUSE_PARTY_VALUE_MAX_HP:
        value = ((PausePartySaveView *)gSaveData)->members[member].max_hp;
        break;
    }
    int divisor = 1;
    for (int digit = 0; digit < digits - 1; ++digit)
        divisor *= 10;
    if (value >= 10 * divisor)
        value = 10 * divisor - 1;
    for (int digit = 0; digit < digits; ++digit) {
        int number = value / divisor;
        if (number || divisor == 1 || leading_zeroes) {
            func_ov007_0207952c(member, x, y, 8 * number + 88, 2, 2, 32, 1);
            leading_zeroes = 1;
        }
        x += 8;
        value %= divisor;
        divisor /= 10;
    }
}

extern "C" void PausePartyBitmap_Rebuild(int member)
{
    {
        // Preserve the native clear wrapper's stack halfword store/load.
        volatile u16 zero = 0;
        MIi_CpuClear16(zero, (u8 *)WORK.owned8c + 4480 * member, 4480);
    }
    int level_x = data_ov007_0208db50[0];
    int level_y = data_ov007_0208db50[1];
    func_ov007_0207952c(member, level_x - 32, level_y + 7, 248, 4, 1, 48, 0);
    PausePartyBitmap_DrawValue(level_x, level_y, member, PAUSE_PARTY_VALUE_LEVEL, 3, 0);
    int hp_x = data_ov007_0208db50[2];
    int hp_y = data_ov007_0208db50[3];
    func_ov007_0207952c(member, hp_x - 30, hp_y + 7, 256, 4, 1, 48, 0);
    PausePartyBitmap_DrawValue(hp_x, hp_y, member, PAUSE_PARTY_VALUE_CURRENT_HP, 3, 0);
    int maximum_x = data_ov007_0208db50[4];
    int maximum_y = data_ov007_0208db50[5];
    func_ov007_0207952c(member, maximum_x - 7, maximum_y, 264, 1, 2, 32, 1);
    PausePartyBitmap_DrawValue(maximum_x, maximum_y, member, PAUSE_PARTY_VALUE_MAX_HP, 3, 0);
}

#include <game/pause_hp_warning.h>
#include <game/menu_spring.h>

extern "C" {
void func_ov005_0206650c(void *);
void func_ov005_020663d8(int);
void func_ov007_020796f4(int column, int width);
void func_ov007_020797c8(int member, int kind);
void func_ov007_02080908(void);
}

extern "C" void PausePartyBitmap_UpdateTransition(PausePartyTransitionTask *task)
{
    /* Prepare two members per update before revealing the new bitmap. */
    switch (task->phase) {
    case 0:
        for (int member = 0; member < 2; ++member) {
            if (WORK.available[member])
                func_ov007_020797c8((u8)member, (u8)task->kind);
        }
        WORK.hp_warning_mode = PAUSE_HP_WARNING_SUSPENDED;
        ++task->phase;
        return;
    case 1:
        for (int member = 2; member < 4; ++member) {
            if (WORK.available[member])
                func_ov007_020797c8((u8)member, (u8)task->kind);
        }
        task->column = 0;
        task->step = 6;
        ++task->phase;
        return;
    case 2:
        if (task->column + task->step > 112)
            task->step = 112 - task->column;
        func_ov007_020796f4(task->column, task->step);
        task->column += task->step;
        if (task->column >= 112)
            func_ov005_0206650c(task);
        return;
    case 100:
        for (int member = 0; member < 2; ++member) {
            if (WORK.available[member])
                PausePartyBitmap_Rebuild(member);
        }
        ++task->phase;
        return;
    case 101:
        for (int member = 2; member < 4; ++member) {
            if (WORK.available[member])
                PausePartyBitmap_Rebuild(member);
        }
        task->step = 6;
        task->column = 112 - task->step;
        ++task->phase;
        return;
    case 102:
        /* The final reverse strip starts at zero and leaves column at -step. */
        if (task->column < 0)
            task->column = 0;
        func_ov007_020796f4(task->column, task->step);
        task->column -= task->step;
        if (task->column == -task->step) {
            WORK.hp_warning_mode = PAUSE_HP_WARNING_ACTIVE;
            func_ov007_02080908();
            func_ov005_0206650c(task);
        }
        return;
    }
}

extern "C" void PausePartyBitmap_StartTransition(int mode)
{
    func_ov005_020663d8(9);
    PausePartyTransitionTask *task = (PausePartyTransitionTask *)func_ov005_0206659c(
        (void (*)(PauseMenuElement *))PausePartyBitmap_UpdateTransition, 5, 1);
    switch (mode) {
    case PAUSE_PARTY_TRANSITION_CLOTHING: task->kind = 2; break;
    case PAUSE_PARTY_TRANSITION_BADGES: task->kind = 3; break;
    case PAUSE_PARTY_TRANSITION_STATUS: task->phase = 100; break;
    }
    MenuSpring_AdvanceActiveChains();
}
