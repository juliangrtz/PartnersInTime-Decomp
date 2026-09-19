#include "pause_scene_internal.h"
#include <game/pause_selection_sprites.h>
#include <game/battle_scene.h>
#include <game/overlay007_party.h>
#include <game/scene_menu_motion.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(void *);
void *func_ov005_02069084(void *, int);
u8 func_ov007_02075408(Overlay7Party *);
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
