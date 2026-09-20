/*
 * Pause list cursor (overlay 7, 0x0207F388-0x0207F5F8).
 *
 * The selected-row cursor and marker, plus scroll arrows at the list edges.
 */

#include "pause_scene_internal.h"
#include <game/pause_selection_sprites.h>
#include <game/battle_scene.h>
#include <game/overlay007_party.h>
#include <game/scene_menu_motion.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(void *);
void *func_ov005_02069084(void *, int);
u8 func_ov007_02075408(Overlay7Party *);
int func_ov007_02075410(Overlay7Party *, int);
int func_ov007_02075460(Overlay7Party *, int);
}
#define PARTY ((Overlay7Party *)data_ov007_0208e1e4)

/* Preserve the Q12 conversion before truncating to pixel coordinates. */
extern "C" void PauseList_UpdateSelectionCursor(PauseMenuElement *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    int x = (data_ov007_020906f0.x + 60) << 12;
    int y = (data_ov007_020906f0.y + 48 + 12 * func_ov007_02075408(PARTY)) << 12;
    sprite->animation_offset_x = x / 4096;
    sprite->animation_offset_y = y / 4096;
    func_ov005_02069084(sprite, 8);
}

extern "C" void PauseList_UpdateSelectionMarker(PauseMenuElement *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    int x = (data_ov007_020906f0.x + 128) << 12;
    sprite->set_primary_animation(8, 0, 1);
    int y = (data_ov007_020906f0.y + 49 + 12 * func_ov007_02075408(PARTY)) << 12;
    sprite->animation_offset_x = x / 4096;
    sprite->animation_offset_y = y / 4096;
    func_ov005_02069084(sprite, 50);
}

static inline void SetPosition(BattleModel *sprite, int x, int y)
{
    sprite->animation_offset_x = x;
    sprite->animation_offset_y = y;
}

extern "C" void PauseList_UpdateScrollArrow(PauseListArrowTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    SetPosition(sprite, task->x / 4096, task->y / 4096);
    if (!task->part) {
        if (!func_ov007_02075460(PARTY, 0)) {
            sprite->set_primary_animation(1, -1, 1);
        } else {
            if (!func_ov007_02075410(PARTY, 0)) {
                sprite->animation_offset_x = -128;
                sprite->animation_offset_y = -128;
            }
            sprite->set_primary_animation(7, -1, 1);
        }
    } else {
        if (!func_ov007_02075460(PARTY, 1)) {
            sprite->set_primary_animation(2, -1, 1);
        } else {
            if (!func_ov007_02075410(PARTY, 1)) {
                sprite->animation_offset_x = -128;
                sprite->animation_offset_y = -128;
            }
            sprite->set_primary_animation(8, -1, 1);
        }
    }
    func_ov005_02069084(sprite, 8);
}
