#include <game/title_panel_motion.h>
#include <game/heap.h>

void TitlePanelResources_Release(TitlePanelResources *resources)
{
    if (resources->pixels) {
        GameHeap_Free(resources->pixels);
        resources->pixels = 0;
    }
    if (resources->palette_data) {
        GameHeap_Free(resources->palette_data);
        resources->palette_data = 0;
    }
    GameTextureAllocation_Unlink(&resources->texture);
    GameTexturePalette_Unlink(&resources->palette);
}
