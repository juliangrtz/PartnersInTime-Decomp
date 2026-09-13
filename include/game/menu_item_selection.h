#ifndef PIT_GAME_MENU_ITEM_SELECTION_H
#define PIT_GAME_MENU_ITEM_SELECTION_H

#include <nitro.h>

/* The selected label's origin is Q12; its signed offsets have eight fractional bits. */
typedef struct MenuItemSelectionOwner {
    u8 unknown_00[42];
    s8 status;
    u8 quantity, unknown_2c, half_width;
    u16 unknown_2e;
    int x, y;
    s16 offset_x, offset_y;
    u8 unknown_3c[12];
} MenuItemSelectionOwner;

/* Each screen has child sprites referring to the same 72-byte owner task. */
typedef struct MenuItemSelectionTask {
    u8 unknown_00[16];
    MenuItemSelectionOwner *parent;
    u8 unknown_14[12];
    int phase, timer, sub_screen;
    /* Label strips use pixel offsets; quantity sprites use a digit and tile base. */
    union { int offset_x; int digit; };
    union { int offset_y; int tile_base; };
    u8 unknown_34[20];
} MenuItemSelectionTask;

typedef char MenuItemSelectionOwnerSize[sizeof(MenuItemSelectionOwner) == 72 ? 1 : -1];
typedef char MenuItemSelectionTaskSize[sizeof(MenuItemSelectionTask) == 72 ? 1 : -1];
typedef char MenuItemSelectionOwnerPosition[(u32)&((MenuItemSelectionOwner *)0)->x == 48 ? 1 : -1];
typedef char MenuItemSelectionOwnerOffset[(u32)&((MenuItemSelectionOwner *)0)->offset_x == 56 ? 1 : -1];
typedef char MenuItemSelectionOwnerQuantity[(u32)&((MenuItemSelectionOwner *)0)->quantity == 43 ? 1 : -1];
typedef char MenuItemSelectionTaskScreen[(u32)&((MenuItemSelectionTask *)0)->sub_screen == 40 ? 1 : -1];
typedef char MenuItemSelectionTaskDigit[(u32)&((MenuItemSelectionTask *)0)->digit == 44 ? 1 : -1];
typedef char MenuItemSelectionTaskTileBase[(u32)&((MenuItemSelectionTask *)0)->tile_base == 48 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void MenuItemSelection_UpdateOffsetSprite(MenuItemSelectionTask *task);
void MenuItemSelection_UpdateLabelStrip(MenuItemSelectionTask *task);
void MenuItemSelection_UpdateQuantitySprite(MenuItemSelectionTask *task);
#ifdef __cplusplus
}
#endif

#endif
