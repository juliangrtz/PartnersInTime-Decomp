#include "pause_scene_internal.h"
#include <game/pause_party_bitmap.h>
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
