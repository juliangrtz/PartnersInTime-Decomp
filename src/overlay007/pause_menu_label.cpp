#include "pause_scene_internal.h"
extern "C" {
#include <game/pause_menu_label.h>
extern GameSpriteAllocation data_ov007_020908d0;
void func_ov007_020758f0(void *, int, int, int, int, int, int, int);
void func_ov005_020663d8(int);
}
extern "C" void PauseMenuLabel_ReleaseRows(void)
{
    func_ov005_020663d8(2);
    GameSpriteAllocation_Unlink(&data_ov007_020908d0);
}
extern "C" void PauseMenuLabel_DrawPage(int page)
{
    int entry = 0;
    switch (page) {
    case 0:
        entry = 0;
        break;
    case 1:
        entry = 1;
        break;
    case 2:
        switch (WORK.party_count) {
        case 1:
            entry = 2;
            break;
        case 2:
            entry = 3;
            break;
        case 4:
            entry = 4;
            break;
        }
        break;
    case 3:
        entry = 5;
        break;
    case 4:
        entry = 6;
        break;
    case 5:
        entry = 6;
        break;
    }
    func_ov007_020758f0(data_ov007_0208e1e4, 640, 2, entry, 7, 1, 0, 1);
}
