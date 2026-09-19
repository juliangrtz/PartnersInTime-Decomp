#include "pause_scene_internal.h"
#include <game/overlay007_party.h>

extern "C" {
#include <game/heap.h>
void MIi_CpuClear16(u16, void *, u32);
void MIi_CpuClearFast(u32, void *, u32);
void MIi_CpuCopy16(const void *, void *, u32);

/* Native clear wrappers materialize a zero on the stack before the call.
 * Inlining keeps those wrapper locals separate from the caller's text buffer
 * and GameTextToken return temporary, as in pause_resources.cpp. */
static inline void ClearFast(void *destination, u32 size)
{
    volatile u32 zero = 0;
    MIi_CpuClearFast(zero, destination, size);
}
static inline void Clear16(void *destination, u32 size)
{
    volatile u16 zero = 0;
    MIi_CpuClear16(zero, destination, size);
}

void *PauseParty_Init(void *storage, void *fonts)
{
    Overlay7Party *party = (Overlay7Party *)storage;
    party->list_images = 0;
    GameText_Init(&party->text, (const u32 *const *)fonts, (u8 *)party + sizeof(*party),
                  0, 0, 0, 1, 3, 1, 6, 31, 0, 255, 0);
    return party;
}

void *PauseParty_Destroy(void *storage)
{
    Overlay7Party *party = (Overlay7Party *)storage;
    if (party->list_images) {
        GameHeap_DeleteArray(party->list_images);
        party->list_images = 0;
    }
    return party;
}

void PauseList_InitGraphics(Overlay7Party *party)
{
    MI_CpuFill8(party->saved_first, 0, sizeof(party->saved_first));
    MI_CpuFill8(party->saved_selected, 0, sizeof(party->saved_selected));
    party->tile_row = 0;
    party->first = 0;
    party->span.offset = 0;
    party->visible = 0;
    void *destination = Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN);
    ClearFast(destination, 4224);
    for (int digit = 0; digit < 10; ++digit) {
        Clear16((u8 *)party + 332, 32);
        Clear16((u8 *)party + 1356, 32);
        u8 character[2];
        character[1] = 0;
        character[0] = digit + '0';
        party->text.cursor.bits.x = 0;
        party->text.cursor.bits.y = 0;
        party->text.text = character;
        GameText_Next(&party->text, 0, 0);
        for (int screen = 0; screen < 2; ++screen) {
            u8 *tiles = (u8 *)Overlay5Display_GetObjVram((DisplayEngine)(u8)screen) + 64 * digit;
            MIi_CpuCopy16((u8 *)party + 332, tiles, 32);
            MIi_CpuCopy16((u8 *)party + 1356, tiles + 32, 32);
        }
    }
}
}
