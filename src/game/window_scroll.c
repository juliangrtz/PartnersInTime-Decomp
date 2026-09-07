#include <game/window.h>

void GameWindow_ApplyScroll(GameWindowManager *manager) {
    if (!manager->display.bits.fixed_main_scroll) {
        switch (manager->state.bits.main_bg) {
        case 0: *(vu32 *)0x04000010 = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 1: *(vu32 *)0x04000014 = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 2: *(vu32 *)0x04000018 = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 3: *(vu32 *)0x0400001c = (manager->main_scroll_x & 0x1ff) | (((manager->main_scroll_y + 16) << 16) & 0x1ff0000); break;
        }
    }
    if (!manager->display.bits.fixed_sub_scroll) {
        switch (manager->state.bits.sub_bg) {
        case 0: *(vu32 *)0x04001010 = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 1: *(vu32 *)0x04001014 = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 2: *(vu32 *)0x04001018 = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        case 3: *(vu32 *)0x0400101c = (manager->sub_scroll_x & 0x1ff) | (((manager->sub_scroll_y + 16) << 16) & 0x1ff0000); break;
        }
    }
}
