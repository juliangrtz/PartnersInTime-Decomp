#include <game/item.h>
static inline UsableItemRecord *GetUsableItem(u16 item) {
    if ((item & ITEM_ID_USABLE_TAG) != ITEM_ID_USABLE_TAG)
        return 0;
    return &((UsableItemRecord *)gItemRecords)[item & ITEM_ID_INDEX_MASK];
}
int ItemEffect_CalculateValue(u16 item, int current, int maximum, int unused) {
    UsableItemRecord *record = GetUsableItem(item);
    switch (record->effect_kind) {
    case 0:
        if (maximum >= current + record->amount)
            maximum = current + record->amount;
        return maximum;
    case 1:
        if (maximum >= current + record->amount)
            maximum = current + record->amount;
        return maximum;
    case 2: {
        int amount = maximum * record->amount + 50;
        if (maximum >= amount / 100)
            maximum = amount / 100;
        return maximum;
    }
    case 3:
        return 0;
    case 4:
    case 5:
    case 6:
        return maximum + (maximum * record->amount + 50) / 100;
    case 7:
        return 0;
    default:
        return 0;
    }
}
