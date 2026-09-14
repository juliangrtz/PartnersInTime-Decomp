#ifndef PIT_GAME_MENU_BADGE_DESCRIPTION_H
#define PIT_GAME_MENU_BADGE_DESCRIPTION_H
#include <nitro.h>

typedef struct MenuBadgeDescriptionOwner {
    u8 unknown_00[60];
    int transition_pending;
    u8 unknown_40[8];
} MenuBadgeDescriptionOwner;

/* The panel owns fourteen strips; all are 72-byte overlay-5 tasks. */
typedef struct MenuBadgeDescriptionTask {
    u8 unknown_00[16];
    union {
        MenuBadgeDescriptionOwner *owner;
        struct MenuBadgeDescriptionTask *panel;
    } parent;
    u8 unknown_14[12];
    int phase, timer, item, x, y, settled;
    u8 unknown_38[16];
} MenuBadgeDescriptionTask;

typedef char MenuBadgeDescriptionOwnerSize[sizeof(MenuBadgeDescriptionOwner) == 72 ? 1 : -1];
typedef char MenuBadgeDescriptionTaskSize[sizeof(MenuBadgeDescriptionTask) == 72 ? 1 : -1];
typedef char MenuBadgeDescriptionOwnerPending[(u32)&((MenuBadgeDescriptionOwner *)0)->transition_pending == 60 ? 1 : -1];
typedef char MenuBadgeDescriptionTaskPosition[(u32)&((MenuBadgeDescriptionTask *)0)->x == 44 ? 1 : -1];
typedef char MenuBadgeDescriptionTaskSettled[(u32)&((MenuBadgeDescriptionTask *)0)->settled == 52 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void MenuBadgeDescription_UpdateStrip(MenuBadgeDescriptionTask *task);
void MenuBadgeDescription_UpdatePanel(MenuBadgeDescriptionTask *task);
#ifdef __cplusplus
}
#endif
#endif
