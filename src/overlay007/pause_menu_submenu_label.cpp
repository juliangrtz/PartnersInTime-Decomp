#include "pause_scene_internal.h"
extern "C" {
#include <game/pause_menu_label.h>
extern u8 gSceneMenuSubmenus[];
void func_ov007_020758f0(void *, int, int, int, int, int, int, int);
}
extern "C" void PauseMenuLabel_DrawSubmenu(void)
{
    func_ov007_020758f0(data_ov007_0208e1e4, 640, 2, gSceneMenuSubmenus[WORK.selected_menu] + 9, 7, 1, 0, 1);
}