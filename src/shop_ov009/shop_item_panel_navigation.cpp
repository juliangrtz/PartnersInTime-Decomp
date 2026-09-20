/* Item-list selection, clamping and wrapping (overlay 9, 0x02070334-0x020704EC).
 * Scrolling redraws the incoming row; a changed item refreshes the help text. */

#include <game/shop_panels.h>
extern "C" {
void func_ov009_020704ec(ShopItemPanel *, int);
void func_ov005_02069bcc(int, u16, u16, u16);
void ShopItemPanel_MoveSelection(ShopItemPanel *panel, int delta, int wrap)
{
    // The native callers supply a nonempty list: both initial lookups precede the count check.
    int previous_item = ShopItemPanel_GetRowItem(panel, (u16)panel->selected.signed_value);
    int previous_row = (panel->first + panel->selected.signed_value) % panel->count;
    if (!ShopItemPanel_GetCount(panel))
        return;
    panel->selected.signed_value += delta;
    if ((u32)panel->count <= panel->visible_rows) {
        if (panel->selected.signed_value < 0) {
            if (wrap)
                panel->selected.signed_value = panel->count - 1;
            else
                panel->selected.signed_value = 0;
        }
        if (panel->selected.signed_value >= panel->count) {
            if (wrap)
                panel->selected.signed_value = 0;
            else
                panel->selected.signed_value = panel->count - 1;
        }
    } else {
        if (panel->selected.signed_value < 0) {
            panel->selected.signed_value = 0;
            if (panel->first > 0 || wrap)
                func_ov009_020704ec(panel, -1);
        }
        if (panel->selected.signed_value >= panel->visible_rows) {
            panel->selected.signed_value = panel->visible_rows - 1;
            int count = panel->count;
            if ((panel->first + panel->selected.signed_value) % count < count - 1 || wrap)
                func_ov009_020704ec(panel, 1);
        }
    }
    if (previous_row != (panel->first + panel->selected.signed_value) % panel->count)
        func_ov005_02069bcc(231, 0, 4, 128);
    u16 item = ShopItemPanel_GetRowItem(panel, (u16)panel->selected.signed_value);
    if (item != previous_item)
        ShopHelpText_Draw(1, item);
}
}
