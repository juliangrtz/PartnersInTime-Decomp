#ifndef PIT_SHOP_PRICE_INTERNAL_H
#define PIT_SHOP_PRICE_INTERNAL_H
#include <game/item.h>

/* Shared price prefix; the four native record tables have different strides. */
struct ShopPriceRecord {
    u8 unknown_00[12];
    u16 price;
    u8 bonus_class;
};
typedef char ShopPriceRecordSizeCheck[sizeof(ShopPriceRecord) == 16 ? 1 : -1];
extern "C" {
extern u8 data_02050290[], data_020505c4[];
}

static inline const ShopPriceRecord *ShopPrice_GetActionRecord(u16 item)
{
    if ((item & ITEM_ID_ACTION_TAG) != ITEM_ID_ACTION_TAG)
        return 0;
    return (const ShopPriceRecord *)&gActionItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const ShopPriceRecord *ShopPrice_GetUsableRecord(u16 item)
{
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return (const ShopPriceRecord *)&gItemRecords[item & ITEM_ID_INDEX_MASK];
}
static inline const ShopPriceRecord *ShopPrice_GetBadgeRecord(u16 item)
{
    if ((item & ITEM_ID_BADGE_TAG) != ITEM_ID_BADGE_TAG)
        return 0;
    return (const ShopPriceRecord *)&data_02050290[20 * (item & ITEM_ID_INDEX_MASK)];
}
static inline const ShopPriceRecord *ShopPrice_GetClothingRecord(u16 item)
{
    if ((item & ITEM_ID_CLOTHING_TAG) != ITEM_ID_CLOTHING_TAG)
        return 0;
    return (const ShopPriceRecord *)&data_020505c4[28 * (item & ITEM_ID_INDEX_MASK)];
}
static inline const ShopPriceRecord *ShopPrice_GetRecord(u16 item)
{
    switch (item & ITEM_ID_TAG_MASK) {
    case ITEM_ID_ACTION_TAG:
        return ShopPrice_GetActionRecord(item);
    case ITEM_ID_USABLE_TAG:
        return ShopPrice_GetUsableRecord(item);
    case ITEM_ID_BADGE_TAG:
        return ShopPrice_GetBadgeRecord(item);
    case ITEM_ID_CLOTHING_TAG:
        return ShopPrice_GetClothingRecord(item);
    default:
        return 0;
    }
}
#endif
