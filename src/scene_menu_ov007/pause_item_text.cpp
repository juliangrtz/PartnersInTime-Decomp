/*
 * Pause item text (overlay 7, 0x02075B04-0x02075E10).
 *
 * The values and text a row shows and the draw that puts them on screen.
 */

#include "pause_scene_internal.h"
#include <game/overlay007_party.h>
extern "C" {
#include <game/save_state_transfer.h>
extern u16 data_ov007_020a6b98;
extern s8 data_ov007_020a6b9c[];
extern const u8 data_ov007_0208dad4[], data_ov007_0208dadc[], data_ov007_0208daec[];
extern const u8 data_ov007_0208e032[], data_ov007_0208e033[];
void func_ov007_020758f0(Overlay7Party *, u32, int, int, int, int, int, u8);

s8 *PauseItem_GetValues(Overlay7Party *party, int kind, u8 *limit)
{
    s8 *values = 0;
    /* Callers use kinds 0..4; the native fallback does not define a limit. */
    int count;
    switch (kind) {
    case 0: values = ((SaveLiveTransferView *)gSaveData)->items; count = 14; break;
    case 1: values = data_ov007_020a6b9c; count = data_ov007_020a6b98; break;
    case 2: values = ((SaveLiveTransferView *)gSaveData)->clothing; count = 33; break;
    case 3: values = ((SaveLiveTransferView *)gSaveData)->badges; count = 41; break;
    case 4: values = ((SaveLiveTransferView *)gSaveData)->bros_items; count = 11; break;
    }
    if (limit)
        *limit = count;
    return values;
}

const u8 *PauseItem_GetText(Overlay7Party *party, int kind, int mode, int item, int plural)
{
    const u8 *result = 0;
    if (kind == 1) {
        if (item == 0) {
            item = 13;
            if (mode == 0) {
                int entry = PauseItem_GetNameId(party, (u8)kind, item);
                if (((SaveLiveTransferView *)gSaveData)->unknown_48c != 1)
                    ++entry;
                return (const u8 *)GameTextResources_GetEntry(data_ov007_0208dadc[kind], (u16)entry);
            }
        } else {
            if (mode != 0) {
                if (mode == 1)
                    item = data_ov007_0208e033[10 * item];
            } else {
                item = data_ov007_0208e032[10 * item];
            }
            mode = 2;
        }
    }
    switch (mode) {
    case 0: {
        int entry = PauseItem_GetNameId(party, (u8)kind, item);
        /* Native callers pass a word; this branch reads its low stack byte. */
        if (data_ov007_0208dad4[kind] > 1 && *(const u8 *)&plural &&
            PauseItem_GetValues(party, kind, 0)[item] != 1)
            ++entry;
        result = (const u8 *)GameTextResources_GetEntry(data_ov007_0208dadc[kind], (u16)entry);
        break;
    }
    case 1:
        result = (const u8 *)GameTextResources_GetEntry(data_ov007_0208daec[kind], (u16)item);
        break;
    case 2: {
        const u8 *base = (const u8 *)WORK.owned80;
        base += ((const u32 *)base)[((SaveLiveTransferView *)gSaveData)->language] & ~3;
        result = base + ((const u32 *)base)[item];
        break;
    }
    }
    return result;
}

void PauseItem_DrawText(Overlay7Party *party, u32 offset, int mode, int item,
                       int width, int height, u8 plural)
{
    int kind = party->kind;
    if ((u8)(kind + 0xfe) <= 1 && !item) {
        if (mode == 0) {
            mode = 2;
            if (kind == 2) item = 11;
            else item = 13;
        }
        if (mode == 1) {
            mode = 2;
            if (kind == 2) item = 12;
            else item = 14;
        }
    }
    if (mode == 1)
        item = PauseItem_GetDescriptionId(party, kind, item);
    func_ov007_020758f0(party, offset, mode, item, width, height, 0, plural);
}
}
