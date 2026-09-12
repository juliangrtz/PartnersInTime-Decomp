#include "title_sequence_internal.h"
#include <game/title_animation.h>
#include <game/heap.h>
#include <game/save_data.h>
extern TitleAnimationSequence *data_ov006_0207c4e0;
void func_ov006_02073bfc(void *), func_ov006_0207393c(void *);
void func_ov006_02072ae4(void *), func_ov006_02072ea8(void *);
void func_ov006_02072768(void *), func_ov006_02072808(void *);
#define SEQUENCE data_ov006_0207c4e0
#define SAVE ((TitleSequenceSave *)gSaveData)
static inline void StoreSelectionFlag(u8 enabled)
{
    /* The native store truncates the input byte before updating bit 6. */
    SAVE->unknown_06 = enabled;
}
void TitleAnimation_ReleaseSequence(void)
{
    if (SEQUENCE->secondary_prompt.selection_override >= 0)
        StoreSelectionFlag(SEQUENCE->secondary_prompt.selection_override == 0);
    else
        StoreSelectionFlag(SEQUENCE->secondary_prompt.prompt.selection == 0);
    TitlePanelResources_Release(&SEQUENCE->panel_resources);
    /* The native loop releases the first panel six times. */
    {
        int i;
        for (i = 0; i < 6; ++i)
            TitlePanel_Release(&SEQUENCE->panels[0]);
    }
    TitleBrightness_Release(&SEQUENCE->brightness);
    {
        int i;
        for (i = 0; i < 25; ++i)
            func_ov006_02073bfc(&SEQUENCE->auxiliary[i]);
    }
    TitlePanelTransition_Release(&SEQUENCE->transition);
    TitleModel_Release(&SEQUENCE->model);
    func_ov006_0207393c(SEQUENCE->unknown_5bc);
    func_ov006_02072ae4(&SEQUENCE->prompt);
    func_ov006_02072ea8(&SEQUENCE->secondary_prompt);
    {
        int side;
        for (side = 0; side < 2; ++side) {
            int slot;
            for (slot = 0; slot < 7; ++slot)
                TitleOrbit_Destroy(&SEQUENCE->orbit[side][slot]);
        }
    }
    TitleLocalizedSequence_Release(&SEQUENCE->localized_sequence);
    TitleSpriteSequence_Release(&SEQUENCE->sprite_sequence);
    {
        int i;
        for (i = 0; i < 4; ++i) {
            TitleModel_Release(&SEQUENCE->children[i]);
            TitleMovingSprite_Destroy(&SEQUENCE->moving[i]);
        }
    }
    func_ov006_02072768(SEQUENCE->unknown_1b30);
    func_ov006_02072808(SEQUENCE->unknown_1b64);
    if (SEQUENCE->characters) {
        GameHeap_Free(SEQUENCE->characters);
        SEQUENCE->characters = 0;
    }
    if (SEQUENCE->palette) {
        GameHeap_Free(SEQUENCE->palette);
        SEQUENCE->palette = 0;
    }
    if (SEQUENCE->left_screen) {
        GameHeap_Free(SEQUENCE->left_screen);
        SEQUENCE->left_screen = 0;
    }
    if (SEQUENCE->right_screen) {
        GameHeap_Free(SEQUENCE->right_screen);
        SEQUENCE->right_screen = 0;
    }
    if (SEQUENCE->final_screen) {
        GameHeap_Free(SEQUENCE->final_screen);
        SEQUENCE->final_screen = 0;
    }
    if (SEQUENCE) {
        GameHeap_Free(SEQUENCE);
        SEQUENCE = 0;
    }
}
int TitleAnimation_AreOrbitsInactive(void)
{
    int side, slot;
    for (side = 0; side < 2; ++side)
        for (slot = 0; slot < 7; ++slot)
            if (!TitleElement_IsInactive(&SEQUENCE->orbit[side][slot]))
                return 0;
    return 1;
}
