#include "title_sequence_internal.h"
#include "title_animation_internal.h"
extern "C" {
#include <game/heap.h>
#include <game/audio.h>
#include <game/save_data.h>
#include <nitro/card.h>
void func_0202cbd4(void *, int, unsigned);
void func_0202cc58(const void *, void *, unsigned);
void func_0202cd2c(const void *, void *, unsigned);
void func_ov006_020726b8(void);
void func_ov006_0206b9ec(TitleAnimationController *, int, u32);
void func_ov006_02073948(void *);
void func_ov006_02072f44(void *, void *, const void *);
void func_ov006_02072b0c(void *, void *, void *, int);
void func_ov006_02073c08(void *);
extern TitleAnimationController *data_ov006_0207c4e4;
extern TitleAnimationSequence *data_ov006_0207c4e0;
}
#define SEQUENCE data_ov006_0207c4e0
#define ALLOCATION ((TitleSequenceAllocation *)SEQUENCE)
#define CONTROLLER data_ov006_0207c4e4
static inline void CopyBytes(const void *source, void *destination, unsigned size)
{
    if ((u32)source < (u32)destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
}
static inline void Append(void *element)
{
    TitleElement_Append((TitleElementLink *)element);
}
extern "C" void TitleAnimation_InitSequence(void)
{
    SEQUENCE = (TitleAnimationSequence *)GameHeap_Allocate(1, sizeof(TitleSequenceAllocation), 0, 0);
    func_0202cbd4(SEQUENCE, 0, (u8 *)(SEQUENCE + 1) - (u8 *)SEQUENCE);
    void *stamp_data = TitleAnimation_ReadArchiveEntry(CONTROLLER, 2, 10, 0, 0, 0, 0);
    CopyBytes((u8 *)stamp_data + 16, ALLOCATION->trail_stamp_pixels, 512);
    if (stamp_data)
        GameHeap_Free(stamp_data);
    SEQUENCE->characters = TitleAnimation_ReadArchiveEntry(CONTROLLER, 2, 6, 1, 0, 1, &SEQUENCE->character_size);
    SEQUENCE->left_screen = TitleAnimation_ReadArchiveEntry(CONTROLLER, 2, 7, 1, 0, 1, 0);
    SEQUENCE->right_screen = TitleAnimation_ReadArchiveEntry(CONTROLLER, 2, 8, 1, 0, 1, 0);
    SEQUENCE->palette = TitleAnimation_ReadArchiveEntry(CONTROLLER, 2, 9, 0, 0, 1, 0);
    SEQUENCE->final_screen = GameHeap_Allocate(1, 2048, 0, 0);
    /* Each output row combines the left and right halves of two screen maps. */
    u8 *left = (u8 *)SEQUENCE->left_screen;
    u8 *right = (u8 *)SEQUENCE->right_screen + 32;
    u8 *final_screen = (u8 *)SEQUENCE->final_screen;
    for (int row = 0; row < 32; ++row) {
        CopyBytes(left, final_screen, 32);
        left += 64;
        CopyBytes(right, final_screen + 32, 32);
        final_screen += 64;
        right += 64;
    }
    DC_FlushRange(SEQUENCE->characters, SEQUENCE->character_size);
    DC_FlushRange(SEQUENCE->left_screen, 2048);
    DC_FlushRange(SEQUENCE->right_screen, 2048);
    DC_FlushRange(SEQUENCE->final_screen, 2048);
    DC_FlushRange(SEQUENCE->palette, 512);
    func_ov006_020726b8();
    TitleScene_LoadBackgrounds();
    func_ov006_0206b9ec(CONTROLLER, 0, 0xC2000033);
    func_ov006_0206b9ec(CONTROLLER, 1, 0xC2000034);
    func_ov006_0206b9ec(CONTROLLER, 2, 0xC0000087);
    Append(&SEQUENCE->brightness);
    TitleBrightness_Init(&SEQUENCE->brightness);
    TitleBrightness_Start(&SEQUENCE->brightness, 32, 16, 0, 0);
    TitleSequenceModel_Init(&SEQUENCE->model);
    Append(&SEQUENCE->model);
    func_ov006_02073948(SEQUENCE->unknown_5bc);
    Append(SEQUENCE->unknown_5bc);
    void *prompt_data = TitleAnimation_ReadArchiveEntry(CONTROLLER, 3, CONTROLLER->language, 0, 0, 1, 0);
    func_ov006_02072f44(&SEQUENCE->rumble_prompt, SEQUENCE->unknown_5bc, prompt_data);
    Append(&SEQUENCE->rumble_prompt);
    if (prompt_data)
        GameHeap_Free(prompt_data);
    func_ov006_02072b0c(&SEQUENCE->prompt, SEQUENCE->unknown_5bc, &SEQUENCE->rumble_prompt, CONTROLLER->language);
    Append(&SEQUENCE->prompt);
    for (int i = 0; i < 25; ++i)
        Append(&SEQUENCE->auxiliary[i]);
    func_ov006_02073c08(SEQUENCE->auxiliary);
    Append(&SEQUENCE->transition);
    TitlePanelTransition_Init(&SEQUENCE->transition, SEQUENCE->characters, SEQUENCE->character_size,
                              SEQUENCE->left_screen, SEQUENCE->right_screen, SEQUENCE->final_screen, SEQUENCE->palette);
    for (int side = 0; side < 2; ++side) {
        for (int slot = 0; slot < 7; ++slot) {
            Append(&SEQUENCE->orbit[side][slot]);
            TitleOrbit_Init(&SEQUENCE->orbit[side][slot], (TitleOrbitChild *)&SEQUENCE->trail_stamp, side, slot);
        }
    }
    TitleTrailStamp_Init(&SEQUENCE->trail_stamp, &SEQUENCE->trail_buffers, ALLOCATION->trail_stamp_pixels, 4, 32, 0);
    Append(&SEQUENCE->trail_buffers);
    TitleTrailBuffers_Init(&SEQUENCE->trail_buffers, SEQUENCE->trail_pixels[0], SEQUENCE->trail_pixels[1]);
    if (CONTROLLER->language) {
        Append(&SEQUENCE->localized_sequence);
        TitleLocalizedSequence_Init(&SEQUENCE->localized_sequence);
        TitleLocalizedSequence_Start(&SEQUENCE->localized_sequence);
    } else {
        Append(&SEQUENCE->sprite_sequence);
        TitleSpriteSequence_Init(&SEQUENCE->sprite_sequence);
        TitleSpriteSequence_Start(&SEQUENCE->sprite_sequence);
    }
    SEQUENCE->participant_mask = TitleAnimation_GetParticipantMask();
    for (int i = 0; i < 4; ++i) {
        if (SEQUENCE->participant_mask & (1 << i)) {
            TitleModel_Load(&SEQUENCE->children[i], 0xC0000087, 3, 0, 1);
            Append(&SEQUENCE->moving[i]);
            TitleMovingSprite_Init(&SEQUENCE->moving[i], &SEQUENCE->children[i], i & 1, i >> 1, SEQUENCE->moving);
        }
    }
    TitlePanelResources_Load(&SEQUENCE->panel_resources);
    for (int i = 0; i < 6; ++i) {
        TitlePanel_Init(&SEQUENCE->panels[i], &SEQUENCE->panel_resources, i);
        Append(&SEQUENCE->panels[i]);
    }
    for (int i = 0; i < 2; ++i) {
        TitlePanel_Init(&SEQUENCE->active_panels[i], &SEQUENCE->panel_resources, i);
        Append(&SEQUENCE->active_panels[i]);
    }
    if (gSaveData)
        ((TitleSequenceSave *)gSaveData)->rumble_enabled = 1;
    GameAudio_LoadMusic(37, 0, 655360);
    GameAudio_ProcessLoading();
    while (GameAudio_IsLoading())
        GameAudio_ProcessLoading();
    GameAudio_LoadMusic(38, 1, 655360);
    GameAudio_ProcessLoading();
    while (GameAudio_IsLoading())
        GameAudio_ProcessLoading();
}
