#include <game/field_background.h>

extern "C" void FieldBackground_SetOrigin(FieldBackground *background, int origin_x, int origin_y)
{
    for (int layer = 0; layer < 3; ++layer) {
        int x = 0;
        int y = 0;
        background->get_layer_parameters((u8)layer, &x, &y);
        if (!(background->configuration->relative_scroll_layers & (1 << layer))) {
            x *= origin_x;
            y *= origin_y;
        } else {
            x += background->scroll_x[layer] + ((origin_x - background->origin_x) << 8);
            y += background->scroll_y[layer] + ((origin_y - background->origin_y) << 8);
        }
        background->set_scroll((u8)layer, x, y);
    }
    background->origin_x = origin_x;
    background->origin_y = origin_y;
}
