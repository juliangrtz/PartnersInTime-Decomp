#include "save_menu_internal.h"

typedef struct SaveMenuResourceView {
    u8 unknown_00[444];
    void *location_characters[3];
} SaveMenuResourceView;
void SaveMenu_FreeResources(void)
{
    int i;
    SaveMenuText *text = data_ov008_0207828c;
    if (text) {
        SaveMenuText_DeleteOwned(text);
        data_ov008_0207828c = 0;
    }
    for (i = 0; i < 3; ++i) {
        if (((SaveMenuResourceView *)data_ov008_02078290)->location_characters[i]) {
            GameHeap_DeleteArray(((SaveMenuResourceView *)data_ov008_02078290)->location_characters[i]);
            ((SaveMenuResourceView *)data_ov008_02078290)->location_characters[i] = 0;
        }
    }
}
