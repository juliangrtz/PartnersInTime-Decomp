/*
 * Party label (overlay 9, 0x0207B4F0-0x0207B574).
 *
 * Draws a party member's label in the equipment comparison.
 */

#include "shop_party_internal.h"

extern "C" void ShopPartyDisplay_DrawLabel(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    ShopPartyMotionTask *parent = (ShopPartyMotionTask *)task->parent;
    if (parent->visible && ((s8 *)data_ov009_0207ea3c)[0x82] > 0) {
        if (parent->mirrored)
            sprite->attributes_0 = (sprite->attributes_0 & ~0xc00) | 0x400;
        else
            sprite->attributes_0 &= ~0xc00;
        sprite->x = parent->x + parent->offset_x;
        sprite->y = parent->y;
        func_ov005_02069084(sprite, 60);
    }
}
