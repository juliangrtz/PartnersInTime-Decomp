/*
 * Badge description panel (overlay 7, 0x0207BEE0-0x0207C140).
 *
 * The panel describing the highlighted badge and the text strip inside it, both
 * updated when the selection moves.
 */

#include "pause_scene_internal.h"
#include <game/battle_scene.h>
#include <game/menu_badge_description.h>
#include <game/menu_item_effect.h>
#include <game/overlay007_party.h>

/* Native applies the 36-byte member stride before the fixed saved-badge offset. */
struct BadgeDescriptionSavedMember { u8 unknown_00[1048]; u8 badge; };
typedef char BadgeDescriptionSavedMemberSize[sizeof(BadgeDescriptionSavedMember) == 1049 ? 1 : -1];
extern "C" {
BattleModel *Overlay5ResourceA_Get(MenuBadgeDescriptionTask *);
MenuItemEffectSprite *Overlay5ResourceB_Get(MenuBadgeDescriptionTask *);
void func_ov005_02069084(void *, int);
void func_ov007_020758f0(Overlay7Party *, u32, int, int, int, int, int, u8);
}

extern "C" void MenuBadgeDescription_UpdatePanel(MenuBadgeDescriptionTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (WORK.unknown127[7]) {
        task->settled = 0;
        return;
    }
    if (!task->parent.owner->transition_pending || !task->settled) {
        if ((s8)WORK.unknown115 < 2)
            task->y = 163840;
        else
            task->y = 458752;
    }
    if (task->timer) {
        --task->timer;
        return;
    }
    if ((s8)WORK.unknown115 & 1)
        model->set_primary_animation(6, 0, 1);
    else
        model->set_primary_animation(5, 0, 1);
    int y = task->y / 4096;
    model->animation_offset_x = 128;
    model->animation_offset_y = 208-y;
    func_ov005_02069084(model, 22);
    const BadgeDescriptionSavedMember *member = (const BadgeDescriptionSavedMember *)
        (gSaveData + sizeof(SavePartyMember)*(u8)WORK.unknown115);
    u16 item = member->badge + 0x3000;
    if (!task->parent.owner->transition_pending)
        task->settled = 1;
    if (!task->parent.owner->transition_pending || !task->settled) {
        if (task->item != item) {
            task->item = item;
            int description = PauseItem_GetDescriptionId((Overlay7Party *)data_ov007_0208e1e4, 3, item);
            func_ov007_020758f0((Overlay7Party *)data_ov007_0208e1e4,
                WORK.sub_allocation.offset, 1, description, 7, 2, 1, 1);
        }
    }
}

extern "C" void MenuBadgeDescription_UpdateStrip(MenuBadgeDescriptionTask *task)
{
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    if (WORK.unknown127[7])
        return;
    if (task->timer) {
        --task->timer;
        return;
    }
    MenuBadgeDescriptionTask *panel = task->parent.panel;
    if (!panel->parent.owner->transition_pending || !panel->settled) {
        sprite->x = panel->x;
        sprite->y = 811008-task->parent.panel->y;
    }
    func_ov005_02069084(sprite, 20);
}
