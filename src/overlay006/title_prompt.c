#include "title_sequence_internal.h"
#include <game/title_sprite_sequence.h>
#include <game/title_model.h>
#include <game/title_animation.h>
#include <game/title_startup.h>
#include <game/audio.h>
#include <game/input.h>
extern const TitlePromptLayout data_ov006_0207b200[3];
extern const TitleMenuItem data_ov006_0207b2f4[][3];
extern u8 data_0205a00c;
extern GameInput data_0206032c;
void func_ov006_0207380c(void *);
void func_ov006_02073818(void *, int);
void func_ov006_02072ee8(void *);
void func_ov006_02073e6c(void *, int, int);
void TitlePrompt_Update(void *element)
{
    TitleSequencePrompt *work = element;
    switch (work->menu.header.state) {
    case TITLE_PROMPT_INACTIVE:
    case TITLE_PROMPT_EXITING:
        break;
    case TITLE_PROMPT_DELAY:
        ++work->menu.header.elapsed;
        if (work->menu.header.elapsed >= work->menu.header.duration) {
            func_ov006_02073818(work, 16);
            work->menu.header.elapsed = 0;
            work->menu.header.state = TITLE_PROMPT_APPEARING;
        }
        break;
    case TITLE_PROMPT_APPEARING:
        ++work->menu.header.elapsed;
        if (work->menu.header.elapsed < 16) {
            work->scale_x = (-64 * work->menu.header.elapsed) / 16 + 320;
            work->scale_y = (work->menu.header.elapsed << 8) / 16;
        } else {
            work->scale_x = 256;
            work->scale_y = 256;
            work->menu.header.state = TITLE_PROMPT_ACTIVE;
        }
        break;
    case TITLE_PROMPT_ACTIVE: {
        int direction;
        if (work->menu.input_disabled)
            break;
        direction = 0;
        if (data_0206032c.pressed & 0x409) {
            GameAudio_PlayEffectDelayed(232, 0, -1);
            switch (work->menu.selection) {
            case TITLE_PROMPT_START_GAME:
            case TITLE_PROMPT_PRESS_START:
                TitleAnimation_BeginExit(0);
                work->menu.header.state = TITLE_PROMPT_EXITING;
                break;
            case TITLE_PROMPT_OPTIONS:
                func_ov006_02072ee8(work->rumble_prompt);
                work->menu.header.state = TITLE_PROMPT_WAIT_RUMBLE;
                break;
            }
        } else if (data_0206032c.pressed & 0x40) {
            direction = -1;
        } else if (data_0206032c.pressed & 0x80) {
            direction = 1;
        }
        if (direction)
            TitleMenuBase_MoveSelection(&work->menu, direction, 4, TITLE_PROMPT_ACTIVE);
        break;
    }
    case TITLE_PROMPT_WAIT_CURSOR:
        if (TitleElement_IsInactive(work->menu.cursor))
            work->menu.header.state = TITLE_PROMPT_ACTIVE;
        break;
    case TITLE_PROMPT_WAIT_RUMBLE:
        if (TitleElement_IsInactive(work->rumble_prompt))
            work->menu.header.state = TITLE_PROMPT_ACTIVE;
        break;
    }
}
static inline void SetPromptScale(BattleModel *model, s16 x, s16 y)
{
    model->scale_x = x;
    model->scale_y = y;
}
void TitlePrompt_Draw(void *element)
{
    TitleSequencePrompt *work = element;
    int i;
    if (work->menu.header.state == TITLE_PROMPT_INACTIVE)
        return;
    if (work->menu.header.state == TITLE_PROMPT_DELAY)
        return;
    SetPromptScale(work->menu.sub_model, work->scale_x, work->scale_y);
    for (i = 0; i < 3; ++i) {
        if (work->layout == data_ov006_0207b200[i].layout) {
            int selector;
            work->menu.header.x = work->item_x[i];
            work->menu.header.y = work->item_y[i];
            work->menu.header.depth = 512;
            selector = data_ov006_0207b200[i].selector;
            if (work->menu.selection == i)
                --selector;
            func_ov006_02073e6c(work, selector, 1);
            TitleModel_Draw(work);
        }
    }
}
void TitlePrompt_Init(TitleSequencePrompt *work, void *cursor, TitleSequenceRumblePrompt *rumble_prompt, int language)
{
    int i;
    TitleMenuBase_Init(&work->menu, 0xC2000034, cursor);
    for (i = 0; i < 3; ++i) {
        work->item_x[i] = data_ov006_0207b200[i].x << 12;
        work->item_y[i] = data_ov006_0207b200[i].y << 12;
    }
    work->rumble_prompt = rumble_prompt;
    if (data_0205a00c) {
        work->menu.selection = 0;
        work->menu.minimum = 0;
        work->menu.maximum = 1;
        work->layout = 0;
    } else {
        work->menu.selection = 2;
        work->menu.minimum = 2;
        work->menu.maximum = 2;
        work->layout = 1;
    }
    work->menu.items = data_ov006_0207b2f4[language];
    work->menu.header.update = TitlePrompt_Update;
    work->menu.header.draw = TitlePrompt_Draw;
    work->menu.header.pass_flags = 255;
}
void TitlePrompt_Start(TitleSequencePrompt *work, int delay)
{
    work->menu.header.elapsed = 0;
    work->menu.header.duration = delay;
    work->menu.header.pass_flags = 0;
    work->menu.header.state = TITLE_PROMPT_DELAY;
}
void TitlePrompt_Release(TitleSequencePrompt *work)
{
    func_ov006_0207380c(work);
}
