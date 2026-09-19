/*
 * Equipped-item markers (overlay 7, 0x02073AF4-0x02073C94).
 *
 * Marks the rows holding items a party member already has equipped.
 */

#include "pause_scene_internal.h"
#include <game/overlay005_resource.h>
#include <game/overlay007_party.h>
#include <game/pause_list_row.h>
#include <game/scene_menu_motion.h>

extern "C" {
void *Overlay5ResourceB_Get(Overlay5ResourceOwner *);
void *func_ov005_02069084(void *, int);
u8 func_ov007_02075400(Overlay7Party *);
u8 func_ov007_020750bc(Overlay7Party *);
u8 func_ov007_02075324(Overlay7Party *, int);
/* The native alias of WORK.displayed at WORK + 0x11A. */
extern u8 data_ov007_0209070a[4];
}

#define PARTY ((Overlay7Party *)data_ov007_0208e1e4)
/* Live save records start at +0x3F8; each party member occupies 36 bytes. */
#define SAVED_MEMBER(index) \
    ((SavePartyMember *)(gSaveData + 0x3f8 + sizeof(SavePartyMember) * (u8)(index)))

extern "C" void PauseEquippedMarker_Update(PauseEquippedMarkerTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    int equipped;
    if (func_ov007_02075400(PARTY) == 2)
        equipped = SAVED_MEMBER(task->member)->experience_to_next_level.equipment.clothing_id;
    else
        equipped = SAVED_MEMBER(task->member)->badge_id;
    if (equipped) {
        int rows = func_ov007_020750bc(PARTY);
        if (rows > 9) rows = 9;
        int row = 0;
        for (; row < rows; ++row) {
            if (equipped == func_ov007_02075324(PARTY, (u8)row)) break;
        }
        if (row != rows) {
            int adjustment = 0;
            if (func_ov007_02075400(PARTY) == 3) {
                /* Adult and baby partners share a badge indicator position. */
                s8 other;
                if (task->member < 2) other = task->member + 2;
                else other = task->member - 2;
                if (!data_ov007_0209070a[other] ||
                    equipped != SAVED_MEMBER(other)->badge_id)
                    adjustment = 1 - sprite->target_y / 4096;
            }
            sprite->x = (data_ov007_020906f0.x + 56) << 12;
            sprite->y = (adjustment + (data_ov007_020906f0.y + 44 + 12 * row)) << 12;
            func_ov005_02069084(sprite, 39);
        }
    }
}
