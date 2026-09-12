#include "title_sequence_internal.h"
#include "title_animation_internal.h"
extern "C" {
#include <game/input.h>
#include <game/audio.h>
extern GameInput data_0206032c;
extern TitleAnimationSequence *data_ov006_0207c4e0;
extern TitleAnimationController *data_ov006_0207c4e4;
extern const u16 data_ov006_0207b0e8[][2], data_ov006_0207b0ea[][2];
extern const s8 data_ov006_0207b0b4[];
void func_ov006_02072af0(void *, int);
void func_ov006_0206e594(TitleMovingSprite *);
void func_ov006_02073bf8(void *);
}
struct TitleSequencePassElement {
    TitleSequencePassElement *next;
    u8 unknown_04[20];
    void (*update)(void *), (*draw)(void *), (*upload)(void *);
    u8 state;
    u8 update_disabled : 1, draw_disabled : 1, upload_disabled : 1, unknown_3_7 : 5;
    u8 unknown_26[2];
};
typedef char TitleSequencePassElementSize[sizeof(TitleSequencePassElement) == 40 ? 1 : -1];
#define SEQUENCE data_ov006_0207c4e0
extern "C" void TitleAnimation_UpdateSequence(void)
{
    /* The alternate title presentation is selected only for language zero. */
    int localized = data_ov006_0207c4e4->language != 0;
    switch (SEQUENCE->phase) {
    case TITLE_SEQUENCE_ACTIVE:
    case TITLE_SEQUENCE_WAIT_AUTO_EXIT:
    case TITLE_SEQUENCE_EXIT_FADE:
    case TITLE_SEQUENCE_SKIP_OUT:
    case TITLE_SEQUENCE_SKIP_RESET:
    case TITLE_SEQUENCE_SKIP_IN:
        break;
    default:
        if (data_0206032c.pressed & 0xC0F) {
            TitleBrightness_Start(&SEQUENCE->brightness, 16, 0, 16, 0);
            GameAudio_StopMusic(14);
            SEQUENCE->phase = TITLE_SEQUENCE_SKIP_OUT;
        }
        break;
    }
    /* Callbacks may change both the linked list and the phase dispatched below. */
    TitleSequencePassElement *element = (TitleSequencePassElement *)SEQUENCE->list.first;
    if (element) {
        do {
            if (element->update && !element->update_disabled)
                element->update(element);
            element = element->next;
        } while (element);
    }
    switch (SEQUENCE->phase) {
    case TITLE_SEQUENCE_DELAY_MUSIC:
        if (++SEQUENCE->timer > 4) {
            GameAudio_SetMusic(37);
            SEQUENCE->timer = 0;
            SEQUENCE->phase = TITLE_SEQUENCE_DELAY_ENTRY;
        }
        break;
    case TITLE_SEQUENCE_DELAY_ENTRY:
        if (++SEQUENCE->timer > 40) {
            for (int side = 0; side < 2; ++side)
                for (int slot = 0; slot < 7; ++slot)
                    TitleOrbit_StartEntry(&SEQUENCE->orbit[side][slot]);
            SEQUENCE->timer = 0;
            SEQUENCE->phase = TITLE_SEQUENCE_WAIT_INITIAL_FADE;
        }
        break;
    case TITLE_SEQUENCE_WAIT_INITIAL_FADE:
        if (TitleElement_IsInactive(&SEQUENCE->brightness))
            SEQUENCE->phase = TITLE_SEQUENCE_WAIT_ENTRY_ORBITS;
        break;
    case TITLE_SEQUENCE_WAIT_ENTRY_ORBITS:
        if (TitleAnimation_AreOrbitsInactive()) {
            for (int i = 0; i < 6; ++i)
                TitlePanel_StartFadeIn(&SEQUENCE->panels[i]);
            SEQUENCE->timer = 16;
            SEQUENCE->phase = TITLE_SEQUENCE_DELAY_PANELS;
        }
        break;
    case TITLE_SEQUENCE_DELAY_PANELS:
        if (SEQUENCE->timer > 0) {
            --SEQUENCE->timer;
        } else {
            SEQUENCE->trail_buffers.header.pass_flags = 255;
            SEQUENCE->transition.draw_flags.value &= ~1;
            SEQUENCE->transition.draw_flags.value &= ~4;
            TitleMovingSprite_StartEntry(&SEQUENCE->moving[0]);
            TitleMovingSprite_StartEntry(&SEQUENCE->moving[1]);
            SEQUENCE->phase = TITLE_SEQUENCE_WAIT_TRANSITION;
        }
        break;
    case TITLE_SEQUENCE_WAIT_TRANSITION:
        if (TitleElement_IsInactive(&SEQUENCE->transition)) {
            for (int side = 0; side < 2; ++side)
                for (int slot = 0; slot < 7; ++slot)
                    TitleOrbit_StartExpansion(&SEQUENCE->orbit[side][slot]);
            SEQUENCE->phase = TITLE_SEQUENCE_WAIT_EXPANSION;
        }
        break;
    case TITLE_SEQUENCE_WAIT_EXPANSION:
        if (TitleAnimation_AreOrbitsInactive()) {
            for (int side = 0; side < 2; ++side)
                for (int slot = 0; slot < 7; ++slot)
                    TitleOrbit_StartPulse(&SEQUENCE->orbit[side][slot]);
            TitleSequenceModel_StartEntry(&SEQUENCE->model);
            SEQUENCE->phase = TITLE_SEQUENCE_WAIT_MODELS;
        }
        break;
    case TITLE_SEQUENCE_WAIT_MODELS:
        if (TitleElement_IsInactive(&SEQUENCE->model)) {
            if ((localized == 1 && TitleElement_IsInactive(&SEQUENCE->localized_sequence)) ||
                (!localized && TitleElement_IsInactive(&SEQUENCE->sprite_sequence))) {
                func_ov006_02072af0(&SEQUENCE->prompt, data_ov006_0207b0e8[localized][0]);
                SEQUENCE->phase = TITLE_SEQUENCE_ACTIVE;
            }
        }
        break;
    case TITLE_SEQUENCE_ACTIVE:
        func_ov006_0206e594(SEQUENCE->moving);
        for (int i = 0; i < 2; ++i)
            TitlePanel_TryStartTrail(&SEQUENCE->active_panels[i], 512);
        if (!GameAudio_GetMusicState()) {
            if (!TitleElement_IsInactive(&SEQUENCE->rumble_prompt))
                SEQUENCE->rumble_prompt_seen = 1;
            SEQUENCE->timer = 0;
            SEQUENCE->phase = TITLE_SEQUENCE_WAIT_AUTO_EXIT;
        }
        break;
    case TITLE_SEQUENCE_WAIT_AUTO_EXIT:
        if (!TitleElement_IsInactive(&SEQUENCE->rumble_prompt))
            SEQUENCE->rumble_prompt_seen = 1;
        if (SEQUENCE->rumble_prompt_seen) {
            if (++SEQUENCE->idle_timer > 1800)
                TitleAnimation_BeginExit(1);
        } else {
            if (++SEQUENCE->timer > 32)
                TitleAnimation_BeginExit(1);
        }
        break;
    case TITLE_SEQUENCE_EXIT_FADE:
        if (TitleElement_IsInactive(&SEQUENCE->brightness))
            TitleAnimation_RequestExit(SEQUENCE->exit_kind);
        break;
    case TITLE_SEQUENCE_SKIP_OUT:
        if (TitleElement_IsInactive(&SEQUENCE->brightness)) {
            for (int i = 0; i < 25; ++i)
                func_ov006_02073bf8(&SEQUENCE->auxiliary[i]);
            TitleSequenceModel_FinishEntry(&SEQUENCE->model);
            for (int side = 0; side < 2; ++side)
                for (int slot = 0; slot < 7; ++slot)
                    TitleOrbit_Reset(&SEQUENCE->orbit[side][slot]);
            TitleLocalizedSequence_Finish(&SEQUENCE->localized_sequence);
            TitleSpriteSequence_Finish(&SEQUENCE->sprite_sequence);
            for (int i = 0; i < 4; ++i) {
                if (SEQUENCE->participant_mask & (1 << i))
                    TitleMovingSprite_StartOrbit(&SEQUENCE->moving[i]);
            }
            SEQUENCE->trail_buffers.header.pass_flags = 255;
            for (int i = 0; i < 6; ++i)
                TitlePanel_StartPulse(&SEQUENCE->panels[i]);
            GameAudio_SetMusic(38);
            SEQUENCE->phase = TITLE_SEQUENCE_SKIP_RESET;
        }
        break;
    case TITLE_SEQUENCE_SKIP_RESET:
        TitlePanelTransition_Skip(&SEQUENCE->transition);
        TitleBrightness_Start(&SEQUENCE->brightness, 16, 16, 0, 0);
        SEQUENCE->phase = TITLE_SEQUENCE_SKIP_IN;
        break;
    case TITLE_SEQUENCE_SKIP_IN:
        if (TitleElement_IsInactive(&SEQUENCE->brightness)) {
            func_ov006_02072af0(&SEQUENCE->prompt, data_ov006_0207b0ea[localized][0]);
            SEQUENCE->phase = TITLE_SEQUENCE_ACTIVE;
        }
        break;
    }
}

extern "C" {
void TitleAnimation_DrawElements(void)
{
    TitleSequencePassElement *element = (TitleSequencePassElement *)SEQUENCE->list.first;
    if (!element)
        return;
    do {
        if (element->draw && !element->draw_disabled)
            element->draw(element);
        element = element->next;
    } while (element);
}
void TitleAnimation_UploadElements(void)
{
    TitleSequencePassElement *element = (TitleSequencePassElement *)SEQUENCE->list.first;
    if (!element)
        return;
    do {
        if (element->upload && !element->upload_disabled)
            element->upload(element);
        element = element->next;
    } while (element);
}
void TitleAnimation_RequestExit(int kind)
{
    data_ov006_0207c4e4->exit_state = 1;
    data_ov006_0207c4e4->exit_kind = kind;
}
}

extern "C" void TitleAnimation_BeginExit(int kind)
{
    SEQUENCE->rumble_prompt.prompt.input_disabled = 1;
    SEQUENCE->prompt.input_disabled = 1;
    TitleBrightness_Start(&SEQUENCE->brightness, 16, 0, data_ov006_0207b0b4[kind], 0);
    GameAudio_StopMusic(16);
    SEQUENCE->exit_kind = kind;
    SEQUENCE->phase = TITLE_SEQUENCE_EXIT_FADE;
}

extern "C" void TitleAnimation_ResetIdleTimer(void)
{
    SEQUENCE->idle_timer = 0;
}
