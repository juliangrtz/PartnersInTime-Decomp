/* Select the pickup sound and delay from the tagged item's collection class. */
#include <game/field_area.h>
extern "C" {
#include <game/audio.h>
#include <game/item.h>
extern u8 data_02050290[], data_020505c4[];
extern const u16 data_ov000_020bfbc4[], data_ov000_020bfbd0[];
}
struct ItemCollectionRecord {
    u8 unknown_00[8];
    u8 sound_class;
};

static inline const ItemCollectionRecord *GetActionRecord(u16 item)
{
    if ((item & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG)
        return 0;
    return (const ItemCollectionRecord *)&gActionItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const ItemCollectionRecord *GetUsableRecord(u16 item)
{
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return (const ItemCollectionRecord *)&gItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const ItemCollectionRecord *GetBadgeRecord(u16 item)
{
    if ((item & ITEM_ID_BADGE_TAG) != ITEM_ID_BADGE_TAG)
        return 0;
    return (const ItemCollectionRecord *)(data_02050290 + 20 * (item & ITEM_ID_INDEX_MASK));
}
static inline const ItemCollectionRecord *GetClothingRecord(u16 item)
{
    if ((item & ITEM_ID_CLOTHING_TAG) != ITEM_ID_CLOTHING_TAG)
        return 0;
    return (const ItemCollectionRecord *)(data_020505c4 + 28 * (item & ITEM_ID_INDEX_MASK));
}
static inline const ItemCollectionRecord *GetCollectionRecord(u16 item)
{
    switch (item & ITEM_ID_TAG_MASK) {
    case ITEM_ID_ACTION_TAG:
        return GetActionRecord(item);
    case ITEM_ID_USABLE_TAG:
        return GetUsableRecord(item);
    case ITEM_ID_BADGE_TAG:
        return GetBadgeRecord(item);
    case ITEM_ID_CLOTHING_TAG:
        return GetClothingRecord(item);
    default:
        return 0;
    }
}
extern "C" void FieldArea_PlayItemCollectionSound(FieldAreaContext *area, int item, int source)
{
    if (item != 65535)
        source += 2 * GetCollectionRecord(item)->sound_class;
    int sound = data_ov000_020bfbd0[source];
    if (sound > 0)
        GameAudio_PlayEffectDelayed(sound, data_ov000_020bfbc4[source], -1);
}
