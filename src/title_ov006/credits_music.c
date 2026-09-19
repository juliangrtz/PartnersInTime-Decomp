#include "credits_transition_internal.h"

/* The parent credits controller uses argument 7 as its music completion flag. */

void CreditsMusic_Update(MenuElement *element)
{
    if (element->parent->arguments[7]) {
        func_ov005_0206650c(element);
        return;
    }
    switch (element->state) {
    case 0:
        if (element->counter)
            --element->counter;
        else {
            GameAudio_SetMusic(40);
            ++element->state;
        }
        break;
    case 1:
        if (GameAudio_GetMusicState() != 1) {
            element->counter = 120;
            ++element->state;
        }
        break;
    case 2:
        if (element->counter)
            --element->counter;
        else {
            GameAudio_LoadMusic(43, 0, 3072);
            ++element->state;
        }
        break;
    case 3:
        if (!GameAudio_IsLoading()) {
            if (!element->parent->arguments[7])
                GameAudio_SetMusic(43);
            element->parent->arguments[7] = 1;
            func_ov005_0206650c(element);
        }
        break;
    }
}

void CreditsMusic_StopAfterDelay(MenuElement *element)
{
    if (element->counter) --element->counter;
    else {
        if (element->parent->arguments[7])
            GameAudio_StopMusic(32);
        element->parent->arguments[7] = 1;
        func_ov005_0206650c(element);
    }
}
