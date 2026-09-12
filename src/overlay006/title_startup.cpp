#include <game/battle_scene.h>
#include <game/battle_effect.h>
#include <game/title_sprite_sequence.h>
#include <game/title_startup.h>
#include <game/title_model.h>
extern "C" {
#include "frontend_scene_internal.h"
}

struct MenuElement {
    u8 unknown_00[32];
    s32 state, counter;
    s32 arguments[8];
};
/* Only the prefixes accessed by the frame task are described here. */
struct TitleAnimationIrqView {
    u8 unknown_00[44];
    u32 frame_requested : 1;
    u32 unknown_01_31 : 31;
};
struct TitleAnimationControllerView {
    GameTask base;
    u8 unknown_18[16];
    TitleAnimationIrqView *irq;
};
typedef char MenuElement_Size[sizeof(MenuElement) == 72 ? 1 : -1];
typedef char TitleAnimationIrqView_Size[sizeof(TitleAnimationIrqView) == 48 ? 1 : -1];
typedef char TitleAnimationControllerView_Size[sizeof(TitleAnimationControllerView) == 44 ? 1 : -1];

extern "C" {
extern BattleModelVTable data_ov006_0207b3c0;
extern GameTaskVTable data_ov006_0207b38c;
void *func_ov006_0206c234(void *, u8, u32, void *);
void *func_ov005_020698dc(int);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void func_ov005_0206650c(MenuElement *);
void Overlay5ResourceA_ApplySelector(MenuElement *);

void TitleStartup_UpdateGreeting(MenuElement *work)
{
    switch (work->state) {
    case 0:
        work->counter = 8;
        ++work->state;
        break;
    case 1:
        if (work->counter)
            --work->counter;
        else {
            REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0x1800;
            REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x400;
            work->counter = 0;
            work->state = 100;
        }
        break;
    case 100:
        work->counter += 2184;
        if (work->counter > 0x10000)
            work->counter = 0x10000;
        func_ov005_0206781c(2, 16 - work->counter / 4096);
        if (work->counter == 0x10000) {
            GameAudio_StartStream(data_ov006_0207c4ec);
            void *resource = func_ov005_020698dc(0);
            MenuElement *sprite = func_ov005_0206659c(Overlay5ResourceA_ApplySelector, 6, 1);
            BattleModel *model = Overlay5ResourceA_Attach(sprite, 0, 1);
            func_ov005_02068908(model, 0, resource, 0, -1);
            model->animation_offset_x = 128;
            model->animation_offset_y = 96;
            model->set_primary_animation(0, 0, 1);
            model->flag_bits.animation_mode = 1;
            sprite->arguments[0] = 59;
            /* Preserve the two native stores forming the 156-frame hold. */
            work->counter = 90;
            work->counter += 66;
            ++work->state;
        }
        break;
    case 101:
        if (!--work->counter)
            ++work->state;
        break;
    case 102:
        work->counter += 2184;
        if (work->counter > 0x10000)
            work->counter = 0x10000;
        func_ov005_0206781c(2, work->counter / 4096);
        if (work->counter == 0x10000) {
            work->counter = 0;
            work->state = 1000;
        }
        break;
    case 1000:
        data_ov006_0207c4e8->phase = 1;
        func_ov005_0206650c(work);
        break;
    }
}

GameIrqTask *TitleSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207b508;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *TitleSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207b508;
    GameIrqTask_DestroyBase(task);
    return task;
}

void TitleAnimationFrameTask_WaitVBlank(GameTask *task)
{
    TitleAnimationControllerView *controller = (TitleAnimationControllerView *)task->argument;
    controller->irq->frame_requested = 1;
    GameFrame_WaitVBlank();
}
GameTask *TitleAnimationFrameTask_Destroy(GameTask *task)
{
    task->vtable = &data_ov006_0207b38c;
    GameTask_DestroyBase(task);
    return task;
}
BattleModel *TitleModelController_Destroy(BattleModel *model)
{
    *(const void **)model = &data_ov006_0207b3c0;
    BattleModelController_DestroyBase(model);
    return model;
}
int TitleElement_IsInactive(const void *element)
{
    return ((const TitleSpriteHeader *)element)->state == 0;
}
void *TitleAnimation_Create(void)
{
    void *controller = GameHeap_New(75304, 0, 0, 0);
    if (controller)
        controller = func_ov006_0206c234(controller, 8, 0, 0);
    return controller;
}
int TitleAnimation_GetParticipantMask(void)
{
    int slot = data_0205e32c.selected;
    int present = data_0205e32c.present;
    if (!(present & (1 << slot))) {
        slot = 1 - slot;
        if (!(present & (1 << slot)))
            return 3;
    }
    const u32 *flags = (const u32 *)SaveStorage_GetSlot((u8)slot);
    if (!((*flags >> 27) & 1)) {
        if ((*flags >> 5) & 1)
            return 3;
        if ((*flags >> 6) & 1)
            return 3;
        if ((*flags >> 7) & 1)
            return 12;
    }
    return 15;
}
}
