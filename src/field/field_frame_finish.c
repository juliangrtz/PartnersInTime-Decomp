#include <game/field_display.h>
#include <game/input.h>
#include <game/sprite_output.h>
extern GameOamEntry data_02060340[], data_02060740[];
void FieldDisplay_FinishFrame(void) {
    GameOam_SortGroups(0);
    GameOam_SortGroups(1);
    GameOam_BuildSorted(0, data_02060340, data_02060340);
    GameOam_BuildSorted(1, data_02060740, data_02060740);
    GameFrame_WaitVBlank();
    GameOam_SetReserved(0, 0, 0, 0);
    GameOam_SetReserved(1, 0, 0, 0);
    GameInput_Read();
}
