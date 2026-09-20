/* Reward-list rendering (overlay 2, 0x0206CED4-0x0206D270).
 * Submit visible item icons and the label layer, then build a panel from its
 * top, repeated middle and bottom sprites. Restore the panel origin afterward.
 * The three renderers use the shared model prefix, not primary-model allocation sizes.
 */
extern "C" {
#include "battle_reward_items_internal.h"
#include <game/item.h>
extern u8 data_02050290[], data_020505c4[], data_02060740[];
void BattleCommonResources_PrepareOam(void);
}
#include <game/battle_scene.h>
extern "C" void func_020093b4(BattleModel *, int);
#define WORK data_ov002_020c06a4
static inline const u8 *ActionRecord(int item)
{
    if ((item & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG)
        return 0;
    return (const u8 *)&gActionItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const u8 *UsableRecord(int item)
{
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return (const u8 *)&gItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const u8 *BadgeRecord(int item)
{
    if ((item & ITEM_ID_BADGE_TAG) != ITEM_ID_BADGE_TAG)
        return 0;
    return &data_02050290[20 * (item & ITEM_ID_INDEX_MASK)];
}
static inline const u8 *ClothingRecord(int item)
{
    if ((item & ITEM_ID_CLOTHING_TAG) != ITEM_ID_CLOTHING_TAG)
        return 0;
    return &data_020505c4[28 * (item & ITEM_ID_INDEX_MASK)];
}

extern "C" void BattleRewardItems_Draw(void)
{
    BattleModel *text;
    int first_object, first_affine, original_y;
    BattleModel *panel = WORK->objects[1].alternate_model;
    int y = panel->animation_offset_y;
    int count = WORK->flags.bits.item_count;
    original_y = y;
    text = WORK->objects[0].alternate_model;
    BattleCommonResources_PrepareOam();
    u8 objects = data_0205a05c[1], affine = data_0205a060[1];
    first_object = objects;
    first_affine = affine;
    func_020093b4(panel, 1);
    func_020093b4(text, 1);
    BattleModel *icon = WORK->objects[2].alternate_model;
    func_020093b4(icon, 1);
    int icon_y = y + 14;
    for (unsigned i = 0; i < WORK->flags.bits.item_count; ++i, icon_y += 16) {
        if (WORK->rows[i].visible) {
            icon->animation_offset_x = 56;
            icon->animation_offset_y = icon_y;
            int item = WORK->rows[i].item;
            const u8 *record;
            switch (item & ITEM_ID_TAG_MASK) {
            case ITEM_ID_ACTION_TAG:
                record = ActionRecord(item);
                break;
            case ITEM_ID_USABLE_TAG:
                record = UsableRecord(item);
                break;
            case ITEM_ID_BADGE_TAG:
                record = BadgeRecord(item);
                break;
            case ITEM_ID_CLOTHING_TAG:
                record = ClothingRecord(item);
                break;
            default:
                record = 0;
            }
            icon->set_primary_animation(record[9], 0, 1);
            icon->draw(data_02060740, &objects, &affine);
        }
    }
    func_020093b4(icon, 0);
    text->draw(data_02060740, &objects, &affine);
    panel->set_primary_animation(0, 0, 1);
    panel->draw(data_02060740, &objects, &affine);
    panel->set_primary_animation(2, 0, 1);
    for (int i = 0; i < count; ++i) {
        y += 16;
        panel->animation_offset_y = y;
        panel->draw(data_02060740, &objects, &affine);
    }
    panel->animation_offset_y = y + 16;
    panel->set_primary_animation(1, 0, 1);
    panel->draw(data_02060740, &objects, &affine);
    GameOam_AddGroup(1, 256, objects - first_object, affine - first_affine);
    func_020093b4(panel, 0);
    func_020093b4(text, 0);
    panel->animation_offset_y = original_y;
}
